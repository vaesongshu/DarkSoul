// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/AI/BaseEnemy.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "../Character/SoulBaseCharacter.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/ArrowComponent.h>
#include "../Logic/DarkSoulEventManager.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemySword"));
	Sword->SetupAttachment(GetMesh(),"SWORD");

	HitColl = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	HitColl->SetupAttachment(Sword);
	HitColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitColl->SetCollisionResponseToChannels(ECR_Ignore);
	HitColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bEquip = false;
	MaxHP = 100.f;
	HP = MaxHP;
	TargetPlayer = nullptr;
	LastAttackAnimsIndex = 0;
	EnemyAttackCollisionRadius = 70.f;
	bAttack = false;
	bRolling = false;
	bInjury = false;
	bRunning = false;
	bIgnoreHit = false;
	bDie = false;
	RandomStrafeValue = 1;
	RushAttackPos = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	GetWorld()->GetTimerManager().SetTimer(DelayFindPlayerPtr, this, &ABaseEnemy::FindTargetPlayer, 1.f, false, 1.f);

	GetWorld()->GetTimerManager().SetTimer(DelayRandomStrafe, this, &ABaseEnemy::EnableStrafe,
		UKismetMathLibrary::RandomFloatInRange(13.f, 19.f), true, UKismetMathLibrary::RandomFloatInRange(13.f, 19.f));
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StrafeMovement(RandomStrafeValue);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseEnemy::GetEnmeyCurHP()
{
	return HP;
}

float ABaseEnemy::GetEnmeyMaxHP()
{
	return MaxHP;
}

void ABaseEnemy::FindTargetPlayer()
{
	while (TargetPlayer == nullptr)
	{
		if (AActor* FindActor = UGameplayStatics::GetActorOfClass(GetWorld(), InsPlayer))
		{
			TargetPlayer = Cast<ASoulBaseCharacter>(FindActor);
		}	
	}		
	if (TargetPlayer)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsObject("TarPlayer", TargetPlayer);
	}
}

void ABaseEnemy::RunningMovement(bool IsRun)
{
	if (IsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

void ABaseEnemy::DrawTheSword()
{
	bEquip = true;
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		CurAnimIns->Montage_Play(EquipAnim[0]);
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WEAPON_R");
		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsEquip", bEquip);
		}
	}
}

void ABaseEnemy::Attack()
{
	if (CanAttack())
	{
		bool RandomAttackBool = UKismetMathLibrary::RandomBoolWithWeight(0.7);
		if (RandomAttackBool)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("TURE"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("FALSE"));
		}

		if (RandomAttackBool)
		{
			bAttack = true;
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRunning", false);
			RunningMovement(false);
			RandomPlayMontage(AttackAnims);
		}
		else
		{
			Rolling();
		}

	}
}

void ABaseEnemy::RushAttack()
{
	if (CanRushAttack())
	{
		if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
		{
			bAttack = true;
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRunning", false);
			RushAttackPos = GetShiftingTargetPos();
			RunningMovement(false);
			CurAnimIns->Montage_Play(RushAttackAnim[0]);
		}
	}
}

void ABaseEnemy::RushAttackShifting(float Lerp)
{
	FVector LerpLoc = UKismetMathLibrary::VLerp(GetActorLocation(), RushAttackPos, Lerp);
	SetActorLocation(FVector(LerpLoc.X, LerpLoc.Y, GetActorLocation().Z));
}

void ABaseEnemy::Rolling()
{
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		bAttack = false;
		bInjury = false;
		bIgnoreHit = true;
		bRolling = true;
		int32 RollingIndex = UKismetMathLibrary::RandomIntegerInRange(0, 5);
		CurAnimIns->Montage_Play(RollingAnims[RollingIndex]);
	}
}

void ABaseEnemy::Die()
{
	bDie = true;
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsAnyoneDie", true);
	//¹Ø±Õ½ºÄÒÌåµÄÅö×²
	if (GetCapsuleComponent())
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	if (GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	}
	DeathUI();
}

void ABaseEnemy::DeathUI()
{
	UDarkSoulEventManager::GetInstancePtr()->OpenFightResultUI.ExecuteIfBound(true);
}

void ABaseEnemy::EnableEnemyAttackCollision()
{
	FVector SpherePos = GetActorLocation() + (GetActorForwardVector() * 70.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> OverlapActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePos, EnemyAttackCollisionRadius, ObjectType, nullptr, IgnoreActors, OverlapActors);

	for (auto AttackPlayer : OverlapActors)
	{
		if (ASoulBaseCharacter* AttackTar = Cast<ASoulBaseCharacter>(AttackPlayer))
		{
			AttackTar->Injury();
		}
	}
}

void ABaseEnemy::Injury()
{
	if (CanInjury())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("EnemyInjury"));
		bInjury = true;
		bAttack = false;
		RandomPlayerMontage(InjuryAnims);
		if (HP <= 10)
		{
			HP -= 10;
			HP = 0;
			Die();
		}
		else
		{
			HP -= 10;
		}
	}

}

bool ABaseEnemy::CanAttack()
{
	return bEquip && !bAttack && !bInjury && !bRolling;
}

bool ABaseEnemy::CanInjury()
{
	if (!bIgnoreHit)
	{
		return true;
	}
	return false;
}

bool ABaseEnemy::CanRushAttack()
{
	return bEquip;
}

void ABaseEnemy::RandomPlayMontage(TArray<UAnimMontage*> NeedRandomAnims)
{
	int32 AttackAnimaIndex = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		if (AttackAnimaIndex != LastAttackAnimsIndex)
		{
			LastAttackAnimsIndex = AttackAnimaIndex;
			CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimaIndex]);
		}
		else
		{
			if (AttackAnimaIndex == 0)
			{
				int32 AddIndexNum = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 2);
				AttackAnimaIndex += AddIndexNum;
				LastAttackAnimsIndex = AttackAnimaIndex;
				CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimaIndex]);
			}
			else
			{
				AttackAnimaIndex--;
				LastAttackAnimsIndex = AttackAnimaIndex;
				CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimaIndex]);
			}
		}
	}
}

void ABaseEnemy::RandomPlayerMontage(TArray<UAnimMontage*> NeedRandomAnims)
{
	int32 AttackAnimaIndex = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimaIndex]);
	}
}

void ABaseEnemy::EnableStrafe()
{
	if (bEquip)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsStrafe", true);
		RunningMovement(false);
		if (UKismetMathLibrary::RandomBool())
		{
			RandomStrafeValue = 1;
		}
		else
		{
			RandomStrafeValue = -1;
		}
	}
}

void ABaseEnemy::StrafeMovement(int32 RandomStrafeDir)
{
	const FRotator LocalRotation = GetActorRotation();
	const FVector LocalVector = FRotationMatrix(FRotator(0, LocalRotation.Yaw, 0)).GetUnitAxis(EAxis::Y);
	AddMovementInput(LocalVector, RandomStrafeDir);
}

FVector ABaseEnemy::GetShiftingTargetPos()
{
	return TargetPlayer->PlayerPosArrow->GetComponentLocation();
}


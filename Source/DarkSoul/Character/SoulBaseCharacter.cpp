// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/Character/SoulBaseCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "../SoulPlayerController.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "../DarkSoulHUD.h"
#include "../UI/UI_FightMainUI.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "../AI/BaseEnemy.h"
#include <Components/CapsuleComponent.h>
#include "../AI/EnemyAIController.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Components/ArrowComponent.h>
#include "../Logic/DarkSoulEventManager.h"

// Sets default values
ASoulBaseCharacter::ASoulBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	PlayerPosArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerPos"));
	PlayerPosArrow->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;


	bRunning = false;
	WeaponType = EWeaponType::MELEE;
	MeleeState = EMeleeState::COMMON;
	PlayerBehavior = EPlayerBehavior::IDLE;


	MaxPlayerHP = 100;
	PlayerHP = MaxPlayerHP;
	MaxStamina = 100.f;
	Stamina = MaxStamina;
	MeleeAttackSubStamina = 10.f;
	SwordAttackSubStamina = 10.f;
	MeleeRollingSubStamina = 15.f;
	SwordRollingSubStamina = 15.f;
	IncreaseStamina = 1.f;
	DesiredRotation = FRotator(0,0,0);
	RollingForwordValue = 0;
	RollingRightValue = 0;
	RollingAnimPlayRate = 1.f;
	bEquip = false;
	LastMeleeIndex = 0;
	LastSwordIndex = 0;
	EnemyTarget = nullptr;
	MeleeSphereCollisionRadius = 70.f;
	bDie = false;
	bIgnoreHit = false;
	SwordDefenseInjurySubStamina = 15.f;
}

// Called when the game starts or when spawned
void ASoulBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASoulBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DesiredRotation = CalculateRotation();

}


void ASoulBaseCharacter::FindEnemyPtr()
{
	EnemyTarget = Cast<ABaseEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), InsEnemy));
}

void ASoulBaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	RollingForwordValue = MovementVector.Y;
	RollingRightValue = MovementVector.X;

	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(Controller))
	{
		const FRotator Rotation = PC->GetControlRotation();

		const FVector ForwardDirection = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASoulBaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}



void ASoulBaseCharacter::Attack()
{

}

void ASoulBaseCharacter::EnableMeleeCollision()
{
	FVector SpherePos = GetActorLocation() + (GetActorForwardVector() * 70.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> OverlapActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePos, MeleeSphereCollisionRadius, ObjectType, nullptr, IgnoreActors, OverlapActors);

	for (auto MeleeAttackEnemy : OverlapActors)
	{
		if (ABaseEnemy* AttackTar = Cast<ABaseEnemy>(MeleeAttackEnemy))
		{
			AttackTar->Injury();
		}
	}
}

void ASoulBaseCharacter::RandomAttack(TArray<UAnimMontage*> NeedRandomAnims, int32& LastAttackAnimsIndex)
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

void ASoulBaseCharacter::RandomPlayAnims(TArray<UAnimMontage*> NeedRandomAnims)
{
	int32 RandomAnimIndex = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		CurAnimIns->Montage_Play(NeedRandomAnims[RandomAnimIndex]);
	}
}

void ASoulBaseCharacter::PlayRollingOrSlideAnim(TArray<UAnimMontage*> RollingOrSlideAnims)
{
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		if (RollingForwordValue == 1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnims[0], RollingAnimPlayRate);
		}
		else if (RollingForwordValue == -1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnims[1], RollingAnimPlayRate);
		}
		else if (RollingRightValue == 1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnims[2], RollingAnimPlayRate);
		}
		else if (RollingRightValue == -1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnims[3], RollingAnimPlayRate);
		}
		else
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnims[0], RollingAnimPlayRate);
		}
	}
}

void ASoulBaseCharacter::Rolling()
{

}

void ASoulBaseCharacter::Slide()
{

}

void ASoulBaseCharacter::Injury()
{

}

void ASoulBaseCharacter::ChangeWeaponFunc()
{

}

void ASoulBaseCharacter::SwordDefense()
{

}

void ASoulBaseCharacter::StopDefense()
{

}

void ASoulBaseCharacter::Run()
{
	if (CanRun())
	{
		bRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void ASoulBaseCharacter::StopRun()
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

bool ASoulBaseCharacter::CanRun()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE)
	{
		return true;
	}
	return false;
}

void ASoulBaseCharacter::Die()
{
	bDie = true;
	EnemyTarget->EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsAnyoneDie", true);
	RemovePlayerInput();
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

void ASoulBaseCharacter::DeathUI()
{
	UDarkSoulEventManager::GetInstancePtr()->OpenFightResultUI.ExecuteIfBound(false);
}

void ASoulBaseCharacter::RemovePlayerInput()
{
	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PlayerMappingContext);
		}
	}
}

float ASoulBaseCharacter::GetCurHP()
{
	return PlayerHP;
}

float ASoulBaseCharacter::GetMaxHP()
{
	return MaxPlayerHP;
}

float ASoulBaseCharacter::GetCurStamina()
{
	return Stamina;
}

float ASoulBaseCharacter::GetMaxStamina()
{
	return MaxStamina;
}

void ASoulBaseCharacter::ShowStaminaNotEnoughText()
{
	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(Controller))
	{
		if (ADarkSoulHUD* HUD = Cast<ADarkSoulHUD>(PC->GetHUD()))
		{
			HUD->GetFightMainUI()->ShowStaminaText();
		}
	}
}

FRotator ASoulBaseCharacter::CalculateRotation()
{
	FVector LastVector = GetCharacterMovement()->GetLastInputVector();
	if (LastVector != FVector(0,0,0))
	{
		return UKismetMathLibrary::MakeRotFromX(LastVector);
	}
	else
	{
		return DesiredRotation;
	}
}

void ASoulBaseCharacter::RInterpRotation()
{
	FRotator RInterpRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), 5.f);
	SetActorRotation(FRotator(0, RInterpRot.Yaw, 0));
}


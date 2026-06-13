// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/Character/SoulPlayerCharacter.h"
#include "../SoulPlayerController.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/BoxComponent.h>
#include "../AI/BaseEnemy.h"
#include "../DrakSoulGameInstance.h"
#include "../Logic/DarkSoulEventManager.h"

ASoulPlayerCharacter::ASoulPlayerCharacter()
{
	JianQiao = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerJianQiao"));
	JianQiao->SetupAttachment(GetMesh(), "JianQiao");

	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerSword"));
	Sword->SetupAttachment(GetMesh(), "Sword");

	JianNia = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JianFeng"));
	JianNia->SetupAttachment(Sword);

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHead"));
	Head->SetupAttachment(GetMesh());

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHair"));
	Hair->SetupAttachment(GetMesh(),"HAIR");

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHat"));
	Hat->SetupAttachment(GetMesh(), "HAIR");

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHand_L"));
	LeftHand->SetupAttachment(GetMesh());

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHand_R"));
	RightHand->SetupAttachment(GetMesh());

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerPants"));
	Pants->SetupAttachment(GetMesh());

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerShoes"));
	Shoes->SetupAttachment(GetMesh());

	SwordColl = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordColl->SetupAttachment(Sword);



	MeleeStateWarToCommon = 10.f;
}

void ASoulPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	if (UDrakSoulGameInstance* GI = Cast<UDrakSoulGameInstance>(GetGameInstance()))
	{
		if (GI->TopCloth)
		{
			GetMesh()->SetSkeletalMeshAsset(GI->TopCloth);
		}
		if (GI->BotCloth)
		{
			Pants->SetSkeletalMeshAsset(GI->BotCloth);
		}
		if (GI->Shoes)
		{
			Shoes->SetSkeletalMeshAsset(GI->Shoes);
		}
		if (GI->Hat)
		{
			Hat->SetSkeletalMeshAsset(GI->Hat);
		}
	}
}

void ASoulPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Stamina < 100.f && PlayerBehavior != EPlayerBehavior::ATTACK && PlayerBehavior != EPlayerBehavior::DEFENSE)
	{
		Stamina += DeltaTime * IncreaseStamina;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("MeleeStateWarToCommon ==== %f"),Stamina));
	}

	if (MeleeState == EMeleeState::PREPARWAR && PlayerBehavior != EPlayerBehavior::ATTACK)
	{
		MeleeStateWarToCommon -= DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("MeleeStateWarToCommon ==== %f"),MeleeStateWarToCommon));
		if (MeleeStateWarToCommon <= 0)
		{
			MeleeState = EMeleeState::COMMON;
			MeleeStateWarToCommon = 10.f;
		}
	}
}

void ASoulPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulBaseCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulBaseCharacter::Look);
		}
		if (RunAction)
		{
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASoulBaseCharacter::Run);
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASoulBaseCharacter::StopRun);
		}
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Attack);
		}
		if (RollingAction)
		{
			EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Rolling);
		}
		if (SlideAction)
		{
			EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Slide);
		}
		if (ChangeWeaponAction)
		{
			EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::ChangeWeaponFunc);
		}
		if (DefenseAction)
		{
			EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::SwordDefense);
			EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Completed, this, &ASoulPlayerCharacter::StopDefense);
		}
	}
}

void ASoulPlayerCharacter::Attack()
{
	switch (WeaponType)
	{
	case EWeaponType::MELEE:
		MeleeAttack();
		break;
	case EWeaponType::SWORD:
		SwordAttack();
		break;
	}
}

void ASoulPlayerCharacter::MeleeAttack()
{
	if (CanMeleeAttack())
	{

		PlayerBehavior = EPlayerBehavior::ATTACK;
		MeleeState = EMeleeState::PREPARWAR;
		MeleeStateWarToCommon = 10.f;
		Stamina -= MeleeAttackSubStamina;
		CameraShakeFeedBack(false);
		RandomAttack(MeleeAttackAnims, LastMeleeIndex);
	}
}

void ASoulPlayerCharacter::SwordAttack()
{
	if (CanSwordAttack())
	{
		PlayerBehavior = EPlayerBehavior::ATTACK;
		Stamina -= SwordAttackSubStamina;
		CameraShakeFeedBack(false);
		RandomAttack(SwordAttackAnims,LastSwordIndex);
	}
}

void ASoulPlayerCharacter::EnableSwordCollision(bool IsOpen)
{
	if (IsOpen)
	{
		if (!SwordColl->OnComponentBeginOverlap.IsBound())
		{
			SwordColl->OnComponentBeginOverlap.AddDynamic(this, &ASoulPlayerCharacter::SwordOverlap);
		}	
	}
	else
	{
		if (SwordColl->OnComponentBeginOverlap.IsBound())
		{
			SwordColl->OnComponentBeginOverlap.RemoveDynamic(this, &ASoulPlayerCharacter::SwordOverlap);
		}
	}
}

void ASoulPlayerCharacter::SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		Enemy->Injury();
	}
}

void ASoulPlayerCharacter::Rolling()
{
	switch (WeaponType)
	{

	case EWeaponType::MELEE:
		MeleeRolling();
		break;
	case EWeaponType::SWORD:
		SwordRolling();
		break;
	}
}

void ASoulPlayerCharacter::MeleeRolling()
{
	if (CanMeleeRolling())
	{	
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= MeleeRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;
		PlayRollingOrSlideAnim(MeleeRollingAnims);
	}
}

void ASoulPlayerCharacter::SwordRolling()
{
	if (CanSwordRolling())
	{
		bIgnoreHit = true;
		PlayerBehavior = EPlayerBehavior::ROLLING;
		Stamina -= SwordRollingSubStamina;
		CameraShakeFeedBack(false);
		PlayRollingOrSlideAnim(SwordRollingAnims);
	}
}

void ASoulPlayerCharacter::Slide()
{
	switch (WeaponType)
	{

	case EWeaponType::MELEE:
		MeleeSlide();
		break;
	case EWeaponType::SWORD:
		SwordSlide();
		break;

	}
}

void ASoulPlayerCharacter::MeleeSlide()
{
	if (CanMeleeSlide())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= MeleeRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;
		PlayRollingOrSlideAnim(MeleeSlideAnims);
	}
}

void ASoulPlayerCharacter::SwordSlide()
{
	if (CanSwordSlide())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= SwordRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;
		PlayRollingOrSlideAnim(SwordSlideAnims);
	}
}

void ASoulPlayerCharacter::ChangeWeaponFunc()
{
	if (CanChangeWeapon())
	{
		if (WeaponType == EWeaponType::MELEE)
		{
			WeaponType = EWeaponType::SWORD;
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,"WEAPON_R");
			UDarkSoulEventManager::GetInstancePtr()->UpdateWeaponTex.ExecuteIfBound(WeaponType);
			bEquip = true;
		}
		else if (WeaponType == EWeaponType::SWORD)
		{
			WeaponType = EWeaponType::MELEE;
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Sword");
			UDarkSoulEventManager::GetInstancePtr()->UpdateWeaponTex.ExecuteIfBound(WeaponType);
			bEquip = false;
		}
	}
}

void ASoulPlayerCharacter::SwordDefense()
{
	if (CanSwordDefense())
	{
		PlayerBehavior = EPlayerBehavior::DEFENSE;
	}
}

void ASoulPlayerCharacter::StopDefense()
{
	PlayerBehavior = EPlayerBehavior::IDLE;
}

void ASoulPlayerCharacter::Injury()
{
	if (Caninjury())
	{
		switch (WeaponType)
		{

		case EWeaponType::MELEE:
			MeleeInjury();
			break;
		case EWeaponType::SWORD:
			SwordInjury();
			break;

		}
	}

}

void ASoulPlayerCharacter::MeleeInjury()
{
	PlayerBehavior = EPlayerBehavior::INJURY;
	CameraShakeFeedBack(true);
	RandomPlayAnims(MeleeInjuryAnims);
	
	if (PlayerHP <= 10)
	{
		PlayerHP -= 10;
		PlayerHP = 0;
		Die();
	}
	else
	{
		PlayerHP -= 10;
	}
}

void ASoulPlayerCharacter::SwordInjury()
{
	
	if (PlayerBehavior == EPlayerBehavior::DEFENSE)
	{
		if (Stamina < SwordDefenseInjurySubStamina)
		{
			DestroyDefense();
		}
		else
		{
			CameraShakeFeedBack(false);
			RandomPlayAnims(SwordDefenseInjuryAnims);
			Stamina -= SwordDefenseInjurySubStamina;
			
		}
	}
	else
	{
		PlayerBehavior = EPlayerBehavior::INJURY;
		CameraShakeFeedBack(true);
		RandomPlayAnims(SwordInjuryAnims);
		if (PlayerHP <= 10)
		{
			PlayerHP -= 10;
			PlayerHP = 0;
			Die();
		}
		else
		{
			PlayerHP -= 10;
		}
	}
}

void ASoulPlayerCharacter::DestroyDefense()
{
	PlayerBehavior = EPlayerBehavior::INJURY;
	CameraShakeFeedBack(true);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		CurAnimIns->Montage_Play(SwordDestroyDefenseAnims);
	}
	Stamina -= SwordDefenseInjurySubStamina;
	if (Stamina < 0)
	{
		Stamina = 0;
	}
}

bool ASoulPlayerCharacter::CanMeleeAttack()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeAttackSubStamina) 
	{
		return true;
	}
	else if (Stamina < MeleeAttackSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanSwordAttack()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordAttackSubStamina)
	{
		return true;
	}
	else if (Stamina < SwordAttackSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanMeleeRolling()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeRollingSubStamina)
	{
		return true;
	}
	else if(Stamina < MeleeRollingSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanSwordRolling()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordRollingSubStamina)
	{
		return true;
	}
	else if (Stamina < SwordRollingSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanMeleeSlide()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeRollingSubStamina)
	{
		return true;
	}
	else if (Stamina < MeleeRollingSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanSwordSlide()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordRollingSubStamina)
	{
		return true;
	}
	else if (Stamina < SwordRollingSubStamina)
	{
		ShowStaminaNotEnoughText();
		return false;
	}
	return false;
}

bool ASoulPlayerCharacter::CanChangeWeapon()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE)
	{
		return true;
	}
	return false;
}

bool ASoulPlayerCharacter::CanSwordDefense()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && WeaponType == EWeaponType::SWORD && !bRunning && Stamina > 0)
	{
		return true;
	}
	return false;
}

bool ASoulPlayerCharacter::Caninjury()
{
	if (!bIgnoreHit)
	{
		return true;
	}
	return false;
}

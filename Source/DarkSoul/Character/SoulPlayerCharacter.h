// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkSoul/Character/SoulBaseCharacter.h"
#include "SoulPlayerCharacter.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UNiagaraComponent;
class UBoxComponent;
UCLASS()
class DARKSOUL_API ASoulPlayerCharacter : public ASoulBaseCharacter
{
	GENERATED_BODY()

public:
	ASoulPlayerCharacter();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//¹¥»÷
	virtual void Attack() override;

	void MeleeAttack();

	void SwordAttack();

	//¿ªÆô/¹Ø±Õµ¶½£Åö×²¼ì²â
	UFUNCTION(BlueprintCallable)
	void EnableSwordCollision(bool IsOpen);

	UFUNCTION()
	void SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//·­¹ö
	virtual void Rolling() override;

	void MeleeRolling();

	void SwordRolling();

	//»¬ÐÐ
	virtual void Slide() override;

	void MeleeSlide();

	void SwordSlide();

	//¸ü»»ÎäÆ÷
	virtual void ChangeWeaponFunc() override;

	//³Ö½£·ÀÓù
	virtual void SwordDefense() override;

	//½áÊø³Ö½£·ÀÓù
	virtual void StopDefense() override;

	//ÊÜ»÷
	virtual void Injury() override;

	void MeleeInjury();

	void SwordInjury();

	//ÆÆ·À
	void DestroyDefense();


	//ÐÐÎªÌõ¼þÅÐ¶¨

	bool CanMeleeAttack();

	bool CanSwordAttack();

	bool CanMeleeRolling();

	bool CanSwordRolling();

	bool CanMeleeSlide();

	bool CanSwordSlide();

	bool CanChangeWeapon();

	bool CanSwordDefense();

	bool Caninjury();

public:
	//½£ÇÊ
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Items,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* JianQiao;

	//½£
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Sword;

	//½£·ç
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* JianNia;

	//Ã±×Ó
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hat;

	//¿ã×Ó
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Pants;

	//×óÊÖ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftHand;

	//ÓÒÊÖ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightHand;

	//Ð¬
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Shoes;

	//Í·
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Head;

	//Í··¢
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	//½£µÄÅö×²¼ì²â×é¼þ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SwordColl;

	//È­·¨¹¥»÷ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = AnimMontage,meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> MeleeAttackAnims;

	//³Ö½£¹¥»÷ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SwordAttackAnims;

	//È­·¨·­¹öÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> MeleeRollingAnims;

	//³Ö½£·­¹öÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SwordRollingAnims;

	//È­·¨·­¸úÍ·ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> MeleeSlideAnims;

	//³Ö½£»¬ÐÐÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SwordSlideAnims;

	//È­·¨ÊÜ»÷ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> MeleeInjuryAnims;

	//³Ö½£ÊÜ»÷ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SwordInjuryAnims;

	//³Ö½£·ÀÓùÊÜ»÷ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> SwordDefenseInjuryAnims;

	//³Ö½£·ÀÓùÆÆ·ÀÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwordDestroyDefenseAnims;


	//´Ó±¸Õ½µ½ÆÕÍ¨×´Ì¬µÄµ¹¼ÆÊ±
	float MeleeStateWarToCommon;
	
};

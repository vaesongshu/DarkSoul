// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../SoulEnumType.h"
#include "BaseEnemy.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UAnimMontage;
class ASoulBaseCharacter;
UCLASS()
class DARKSOUL_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Items)
	USkeletalMeshComponent* Sword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	UBoxComponent* HitColl;

	//拔剑动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimMontage)
	TArray<UAnimMontage*> EquipAnim;

	//攻击动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimMontage)
	TArray<UAnimMontage*> AttackAnims;

	//冲刺攻击
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimMontage)
	TArray<UAnimMontage*> RushAttackAnim;

	//受击动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimMontage)
	TArray<UAnimMontage*> InjuryAnims;

	//翻滚动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimMontage)
	TArray<UAnimMontage*> RollingAnims;

	//是否已经装备
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bEquip;

	//是否跑步
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bRunning;

	//是否攻击
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bAttack;

	//是否翻滚
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bRolling;

	//是否受击
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bInjury;

	//是否无敌
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bIgnoreHit;

	//是否死亡
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bDie;

	//血量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Attribute)
	float HP;

	//最大血量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Attribute)
	float MaxHP;

	UFUNCTION(BlueprintPure,BlueprintCallable)
	float GetEnmeyCurHP();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetEnmeyMaxHP();

	class AEnemyAIController* EnemyAIController;

	//玩家目标
	UPROPERTY(BlueprintReadOnly, Category = Attribute)
	ASoulBaseCharacter* TargetPlayer;

	//玩家参照
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute)
	TSubclassOf<ASoulBaseCharacter> InsPlayer;

	void FindTargetPlayer();

	void RunningMovement(bool IsRun);

	//拔刀
	UFUNCTION(BlueprintCallable)
	void DrawTheSword();

	void Attack();

	void RushAttack();

	UFUNCTION(BlueprintCallable)
	void RushAttackShifting(float Lerp);

	void Rolling();

	void Die();

	void DeathUI();

	UFUNCTION(BlueprintCallable)
	void EnableEnemyAttackCollision();

	void Injury();

	bool CanAttack();

	bool CanInjury();

	bool CanRushAttack();

	int32 LastAttackAnimsIndex;

	float EnemyAttackCollisionRadius;

	void RandomPlayMontage(TArray<UAnimMontage*> NeedRandomAnims);

	//随机播放动画
	void RandomPlayerMontage(TArray<UAnimMontage*> NeedRandomAnims);

	//打开观察者模式
	void EnableStrafe();

	//观察者模式（左右侧边走）
	void StrafeMovement(int32 RandomStrafeDir);

	//观察者模式移动方向
	int32 RandomStrafeValue;

	//冲刺攻击的目标位置
	FVector RushAttackPos;

	FVector GetShiftingTargetPos();

	FTimerHandle DelayRandomStrafe;
	FTimerHandle DelayFindPlayerPtr;
};

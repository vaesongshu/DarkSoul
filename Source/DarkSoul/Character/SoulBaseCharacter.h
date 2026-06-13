// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../SoulEnumType.h"
#include "SoulBaseCharacter.generated.h"

class UInputAction;
class ABaseEnemy;
UCLASS()
class DARKSOUL_API ASoulBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	//按键相关
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = PlayerInput)
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* RollingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* ChangeWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	UInputAction* DefenseAction;

	UPROPERTY(BlueprintReadOnly, Category = PlayerInput)
	ABaseEnemy* EnemyTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerInput)
	TSubclassOf<ABaseEnemy> InsEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* PlayerPosArrow;

	void FindEnemyPtr();

	//移动
	void Move(const FInputActionValue& Value);

	//视角移动
	void Look(const FInputActionValue& Value);

	//攻击
	virtual void Attack();

	//拳法攻击检测
	UFUNCTION(BlueprintCallable)
	void EnableMeleeCollision();

	//随机播放攻击动画
	void RandomAttack(TArray<UAnimMontage*> NeedRandomAnims,int32& LastAttackAnimsIndex);

	//随机播放动画
	void RandomPlayAnims(TArray<UAnimMontage*> NeedRandomAnims);

	//播放翻滚/滑行动画
	void PlayRollingOrSlideAnim(TArray<UAnimMontage*> RollingOrSlideAnims);

	//翻滚
	virtual void Rolling();

	//滑行（近战：翻跟头/持剑：滑行）
	virtual void Slide();

	//受击
	virtual void Injury();

	//替换武器
	virtual void ChangeWeaponFunc();

	//持剑防御
	virtual void SwordDefense();

	//结束持剑防御
	virtual void StopDefense();

	//奔跑
	void Run();

	//停止奔跑
	void StopRun();

	bool CanRun();

	//死亡
	void Die();

	//死亡UI事件
	void DeathUI();

	void RemovePlayerInput();
	
	//获取当前生命值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetCurHP();

	//获取人物最大生命值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetMaxHP();

	//是否在奔跑
	UPROPERTY(BlueprintReadOnly, Category = Attribute)
	bool bRunning;

	//是否装备武器
	UPROPERTY(BlueprintReadOnly, Category = Attribute)
	bool bEquip;

	//是否死亡
	UPROPERTY(BlueprintReadOnly, Category = Attribute)
	bool bDie;

	//是否处于无敌
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	bool bIgnoreHit;

	//武器类型
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	EWeaponType WeaponType;

	//徒手状态
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	EMeleeState MeleeState;

	//玩家行为
	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	EPlayerBehavior PlayerBehavior;

	//生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float PlayerHP;

	//最大生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float MaxPlayerHP;

	//体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float Stamina;

	//最大体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float MaxStamina;

	//拳法攻击减少的体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float MeleeAttackSubStamina;

	//持剑攻击减少的体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float SwordAttackSubStamina;

	//持剑防御受击减少的体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float SwordDefenseInjurySubStamina;

	//拳法翻滚减少的体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float MeleeRollingSubStamina;

	//持剑翻滚减少的体力值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float SwordRollingSubStamina;

	//体力值回复系数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float IncreaseStamina;

	//近战攻击检测半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attribute)
	float MeleeSphereCollisionRadius;

	//获取当前人物的体力值
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurStamina();

	//获取当前人物最大体力值
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxStamina();


	UFUNCTION(BlueprintImplementableEvent)
	void CameraShakeFeedBack(bool IsOpen);

	//显示体力值不够的文本提示
	void ShowStaminaNotEnoughText();

	//攻击时转向的目标值
	FRotator DesiredRotation;

	//计算按键的方向
	FRotator CalculateRotation();

	UFUNCTION(BlueprintCallable)
	void RInterpRotation();

	//上一次拳法攻击的序号
	int32 LastMeleeIndex;

	//上一次持剑攻击的序号
	int32 LastSwordIndex;

	//翻滚的朝向值（前后）
	int32 RollingForwordValue;

	//翻滚的朝向值（左右）
	int32 RollingRightValue;

	//翻滚动画播放速率
	float RollingAnimPlayRate;

};

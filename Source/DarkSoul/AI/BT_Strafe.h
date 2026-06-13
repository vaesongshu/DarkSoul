// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Strafe.generated.h"

/**
 * 
 */
class AEnemyAIController;
class ABaseEnemy;
UCLASS()
class DARKSOUL_API UBT_Strafe : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	AEnemyAIController* AIController;

	ABaseEnemy* AICharacter;

	FTimerHandle DelayEndStrafe;

	void EndStrafe();
};

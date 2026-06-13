// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/AI/BT_Running.h"
#include "EnemyAIController.h"
#include "BaseEnemy.h"

EBTNodeResult::Type UBT_Running::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (ABaseEnemy* AICharacter = Cast<ABaseEnemy>(AIController->GetPawn()))
		{
			AICharacter->RunningMovement(true);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

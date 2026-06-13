// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/AI/BT_Attack.h"
#include "EnemyAIController.h"
#include "BaseEnemy.h"

EBTNodeResult::Type UBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("45353534535345"));
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (ABaseEnemy* AICharacter = Cast<ABaseEnemy>(AIController->GetPawn()))
		{
			AICharacter->Attack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

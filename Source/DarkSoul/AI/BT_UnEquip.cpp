// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/AI/BT_UnEquip.h"
#include "EnemyAIController.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "../Character/SoulBaseCharacter.h"

EBTNodeResult::Type UBT_UnEquip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (ASoulBaseCharacter* Player = Cast<ASoulBaseCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject("TarPlayer")))
		{
			AIController->SetFocus(Player);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

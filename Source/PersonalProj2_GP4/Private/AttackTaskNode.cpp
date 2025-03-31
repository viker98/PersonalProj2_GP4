// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTaskNode.h"
#include "BehaviorTree/BTNode.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "Main_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AAIController* OwningAIC = OwnerComp.GetAIOwner())
	{
		Cast<AEnemyCharacter>(OwningAIC->GetCharacter())->PlayAttackAnim();
		return EBTNodeResult::Succeeded;

	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
}

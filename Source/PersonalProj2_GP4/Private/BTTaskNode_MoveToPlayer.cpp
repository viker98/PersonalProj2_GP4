// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MoveToPlayer.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_MoveToPlayer::UBTTaskNode_MoveToPlayer()
{
	NodeName = TEXT("Find Random Location");


}

EBTNodeResult::Type UBTTaskNode_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};

	AAIController* AIController = OwnerComp.GetAIOwner();
    const APawn* AIPawn = AIController->GetPawn();


	const FVector Origin = AIPawn->GetActorLocation();

	const UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(navSystem) && navSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(RandomLocationName, Location);

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
}

FString UBTTaskNode_MoveToPlayer::GetStaticDescription() const
{
	return FString();
}

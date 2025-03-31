// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UBTTaskNode_MoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_MoveToPlayer();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere)
	float SearchRadius;

	FName RandomLocationName = "RandomLocation";

};

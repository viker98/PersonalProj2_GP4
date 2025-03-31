// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;


	UFUNCTION()
	void OnPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);
	//void OnPerceptionForgotten(AActor* TargetActor);

	void SetCurrentTarget(AActor* NewTarget);
	UObject* GetCurrentTarget();

	UBehaviorTree* GetBehaviorTree();

	FName playerLocationName = "PlayerRef";
	FName lastKnownLocationName = "LastKnownLocation";

};

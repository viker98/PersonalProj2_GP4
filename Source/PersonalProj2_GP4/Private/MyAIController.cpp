// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"

AMyAIController::AMyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Componnent");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;

	SightConfig->PeripheralVisionAngleDegrees = 120.f;
	SightConfig->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
	//AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AMyAIController::OnPerceptionForgotten);

}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

void AMyAIController::OnPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (!GetCurrentTarget() && TargetActor == UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn())
		{
			SetCurrentTarget(TargetActor);
		}
	}
	else
	{
		FVector LastKnownLocation = TargetActor->GetActorLocation();
		GetBlackboardComponent()->SetValueAsObject(playerLocationName,nullptr);
		GetBlackboardComponent()->SetValueAsVector(lastKnownLocationName, LastKnownLocation);
	}
}
/*
void AMyAIController::OnPerceptionForgotten(AActor* TargetActor)
{
	UE_LOG(LogTemp,Warning, TEXT("SDJFSLK:DFJ"))

	if (TargetActor == GetCurrentTarget())
	{
		UE_LOG(LogTemp,Warning, TEXT("SDJFSLK:DFJ"))
		SetCurrentTarget(nullptr);
	}
}
*/

void AMyAIController::SetCurrentTarget(AActor* NewTarget)
{

	if (!GetBlackboardComponent())
	{
		return;
	}
	if (NewTarget)
	{
		GetBlackboardComponent()->SetValueAsObject(playerLocationName,NewTarget);
	}
}

UObject* AMyAIController::GetCurrentTarget()
{
	if (!GetBlackboardComponent())
	{
		return nullptr;
	}

	return GetBlackboardComponent()->GetValueAsObject(playerLocationName);
}

UBehaviorTree* AMyAIController::GetBehaviorTree()
{
	return BehaviorTree;
}



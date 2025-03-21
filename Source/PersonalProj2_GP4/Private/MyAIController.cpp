// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Perception/AIPerceptionComponent.h"

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
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathWidget.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/GameplayStatics.h"

void UDeathWidget::NativeConstruct()
{
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UDeathWidget::ExitButtonPressed);
	}
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UDeathWidget::RestartButtonPressed);
	}
}

void UDeathWidget::ExitButtonPressed()
{
	UE_LOG(LogTemp,Warning, TEXT("QuitGame"))
	FGenericPlatformMisc::RequestExit(false);
}

void UDeathWidget::RestartButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading level"))
	UGameplayStatics::OpenLevel(this, levelName);
	APlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController();

}

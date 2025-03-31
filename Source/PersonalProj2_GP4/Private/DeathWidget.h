// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDeathWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;
	
	UFUNCTION()
	void ExitButtonPressed();



	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton;
	
	UFUNCTION()
	void RestartButtonPressed();


	FName levelName = "ThirdPersonMap";
};

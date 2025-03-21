// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PERSONALPROJ2_GP4_API UPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	//UPlayerUserWidget();
	
	void SetHealth(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;
};

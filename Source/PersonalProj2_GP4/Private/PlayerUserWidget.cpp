// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUserWidget.h"



/*
UPlayerUserWidget::UPlayerUserWidget()
{
	HealthBar = CreateWidget<UProgressBar>("Health Bar");
}
*/

void UPlayerUserWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/ MaxHealth);
	}
}

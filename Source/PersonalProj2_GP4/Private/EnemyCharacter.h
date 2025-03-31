// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main_Character.h"
#include "Components/WidgetComponent.h"
#include "PlayerUserWidget.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyCharacter : public AMain_Character
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	void UpdateHud();

	void PlayAttackAnim();

private:
	virtual void OnDead() override;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* overheadWidget;


};

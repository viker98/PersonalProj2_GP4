// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "MyAIController.h"
#include "PlayerUserWidget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
AEnemyCharacter::AEnemyCharacter()
{
	overheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	
	overheadWidget->AttachToComponent(GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);

	
}

void AEnemyCharacter::UpdateHud()
{
	UUserWidget* hud = overheadWidget->GetUserWidgetObject();
	Cast<UPlayerUserWidget>(hud)->SetHealth(health, MaxHealth);
	//Cast<UPlayerUserWidget>(overheadWidget)->SetHealth(health, MaxHealth);
}

void AEnemyCharacter::PlayAttackAnim()
{
	PlayAnimMontage(AttackMontage);
}


void AEnemyCharacter::OnDead()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("CAPUSLE: %s"), *GetCapsuleComponent()->GetName());


	AController* enemyController = GetController();
	Cast<AMyAIController>(Controller)->GetBrainComponent()->StopLogic("Dead");

	RemoveAllEquipment();
}

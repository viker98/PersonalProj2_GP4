// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Equipment.h"
#include "Main_Character.generated.h"

UCLASS()
class AMain_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation3s")
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly)
	float health;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void AddEquipment(TSubclassOf<AEquipment> equipmentToSpawn);
	void RemoveEquipment(AEquipment* equipmentToRemove);
	void Damage(int damageTaken);
	void RemoveAllEquipment();


	UFUNCTION(BlueprintCallable)
	void HitDetect();

protected:

	UPROPERTY(VisibleAnywhere)
	TArray<AEquipment*> CurrentEquipment;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEquipment>> StartingEquipment;


	virtual void OnDead();

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

};

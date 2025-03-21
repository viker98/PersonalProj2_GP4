// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_Character.h"

// Sets default values
AMain_Character::AMain_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMain_Character::BeginPlay()
{
	Super::BeginPlay();
	
	for(TSubclassOf<AEquipment> equipment : StartingEquipment)
	{
		AddEquipment(equipment);
	}

}

// Called every frame
void AMain_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMain_Character::AddEquipment(TSubclassOf<AEquipment> equipmentToSpawn)
{

	FActorSpawnParameters spawnParams;
	AEquipment* spawnedEquipment = GetWorld()->SpawnActor<AEquipment>(equipmentToSpawn, spawnParams);

	FTransform socketLocation = GetMesh()->GetSocketTransform(spawnedEquipment->GetSocketName());
	spawnedEquipment->SetActorTransform(socketLocation);

	FAttachmentTransformRules attachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	spawnedEquipment->AttachToComponent(GetMesh(), attachRules, spawnedEquipment->GetSocketName());
}

void AMain_Character::TakeDamage(int damageTaken)
{
	health -= damageTaken;

	}


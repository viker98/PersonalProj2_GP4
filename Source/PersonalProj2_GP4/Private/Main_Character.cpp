// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_Character.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyCharacter.h"

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

	CurrentEquipment.Add(spawnedEquipment);
}

void AMain_Character::RemoveEquipment(AEquipment* equipmentToRemove)
{
	for (AEquipment* equipment : CurrentEquipment)
	{
		if (equipment == equipmentToRemove)
		{
			CurrentEquipment.Remove(equipment);
			equipment->Destroy();
		}
	}
}

void AMain_Character::Damage(int damageTaken)
{
	health -= damageTaken;
	if (health <= 0)
	{
		OnDead();
		PlayAnimMontage(DeathMontage);
	}
}

void AMain_Character::RemoveAllEquipment()
{
	for (AEquipment* equipment : CurrentEquipment)
	{
		CurrentEquipment.Remove(equipment);
		equipment->Destroy();
	}
}

void AMain_Character::HitDetect()
{
	TArray<FHitResult> outHits;

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);
	TSet<AActor*> AlreadyHitActors;

	bool bHit = GetWorld()->SweepMultiByChannel(
		outHits,
		CurrentEquipment[0]->GetActorLocation(),
		CurrentEquipment[0]->GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(50),
		queryParams
	);

	if(bHit)
	{
		for (const FHitResult& Hit : outHits)
		{

			AActor* HitActor = Hit.GetActor();
			if (AlreadyHitActors.Contains(HitActor))
			{

				return;
			}
			else if(APlayerCharacter* playerRef = Cast<APlayerCharacter>(HitActor))
			{
				if (playerRef->GetIsRolling())
				{
					return;
				}

				if (playerRef->GetIsBlocking())
				{
					playerRef->Damage(CurrentEquipment[0]->GetDamage() /2);
					AlreadyHitActors.Add(playerRef);
				}
				else
				{
					playerRef->Damage(CurrentEquipment[0]->GetDamage());
					AlreadyHitActors.Add(playerRef);
				}
				playerRef->UpdateHud();

			}
			else if (AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(HitActor))
			{
				AlreadyHitActors.Add(HitActor);
				UE_LOG(LogTemp,Warning,TEXT("DAMAGE"))
				EnemyRef->Damage(CurrentEquipment[0]->GetDamage());
				EnemyRef->UpdateHud();
			}
		}
	}
}

void AMain_Character::OnDead()
{
	//override in child class :3
}


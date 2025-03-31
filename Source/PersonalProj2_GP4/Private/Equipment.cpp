// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyClass.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

// Sets default values
AEquipment::AEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Componenent");
	
	SetRootComponent(sceneRootComponent);

	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEquipment::OnEnter);
	boxCollider->OnComponentEndOverlap.AddDynamic(this, &AEquipment::OnLeave);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComp->SetupAttachment(boxCollider);
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{

	Super::BeginPlay();
	if (IsAWeapon)
	{
		SocketName = "Right_HandSocket";
	}
	else
	{
		SocketName = "Left_HandSocket";
	}
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName AEquipment::GetSocketName()
{
	return SocketName;
}

USkeletalMeshComponent* AEquipment::GetMesh()
{
	return SkeletalMeshComp;
}

void AEquipment::OnEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	if (AMain_Character* hitCharacter = Cast<AMain_Character>(OtherActor)) 
	{
		if(this->GetAttachParentActor() != OtherActor)
		{
			boxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FTimerHandle MontageTimerHandle;
			GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &AEquipment::ResetCollision, .5f);
			
			if(APlayerCharacter* playerRef = Cast<APlayerCharacter>(OtherActor))
			{
				playerRef->UpdateHud();
				if (playerRef->GetIsBlocking())
				{
					hitCharacter->TakeDamage(damage/2);
				}
				else
				{
					hitCharacter->TakeDamage(damage);
				}

			}
			else if (AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(OtherActor))
			{
				EnemyRef->UpdateHud();
				hitCharacter->TakeDamage(damage);
			}
		}
	}
	*/
	


	/*
	else if(APlayerCharacter* hitPlayer = Cast<APlayerCharacter>(OtherActor))
	{
		//hitPlayer->TakeDamage(damage);
	}
	*/
}

void AEquipment::OnLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

float AEquipment::GetDamage()
{
	return damage;
}

void AEquipment::ResetCollision()
{
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}









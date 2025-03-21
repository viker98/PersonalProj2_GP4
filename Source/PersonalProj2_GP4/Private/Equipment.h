// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Equipment.generated.h"

UCLASS()
class AEquipment : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FName GetSocketName();
	USkeletalMeshComponent* GetMesh();
	/*
	UFUNCTION()
	void OnBoxComponentHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalizeImpulse,
		const FHitResult& Hit);
	*/

	UFUNCTION()
	void OnEnter(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(EditDefaultsOnly, Category = "SkeletalMesh")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* boxCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	bool IsAWeapon;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* sceneRootComponent;

	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "VALUE")
	float damage;

	void ResetCollision();

};




// Copyright Epic Games, Inc. All Rights Reserved.

#include "PersonalProj2_GP4Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APersonalProj2_GP4Character

APersonalProj2_GP4Character::APersonalProj2_GP4Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}



void APersonalProj2_GP4Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	for (TSubclassOf<AEquipment> equipment : startingEquipment)
	{
		AddEquipment(equipment);
	}
}

/*******************************************************\
|                     My own stuff                      |
/*******************************************************/
void APersonalProj2_GP4Character::Attack()
{
	
	float MontageTime = PlayAnimMontage(AttackMontage);
	DisableInput(ControllerComp);
	FTimerHandle MontageTimerHandle;
	GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &APersonalProj2_GP4Character::FinishedAnimation, MontageTime - 0.4f);
	
}
void APersonalProj2_GP4Character::Roll()
{
	isRolling = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	float MontageTime = PlayAnimMontage(RollMontage);
	DisableInput(ControllerComp);
	FTimerHandle MontageTimerHandle;
	GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &APersonalProj2_GP4Character::FinishedAnimation, MontageTime - 0.7f);
	
}
void APersonalProj2_GP4Character::Block()
{
	PlayAnimMontage(BlockMontage);
	UE_LOG(LogTemp, Warning, TEXT("BLOCKING"))
}
void APersonalProj2_GP4Character::UnBlock()
{
	StopAnimMontage(BlockMontage);
	UE_LOG(LogTemp, Warning, TEXT("END BLOCKING"))

}
void APersonalProj2_GP4Character::FinishedAnimation()
{
	EnableInput(ControllerComp);

	if (isRolling)
	{
		isRolling = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void APersonalProj2_GP4Character::AddEquipment(TSubclassOf<AEquipment> equipmentToSpawn)
{

	FActorSpawnParameters spawnParams;
	AEquipment* spawnedEquipment = GetWorld()->SpawnActor<AEquipment>(equipmentToSpawn,spawnParams);

	FTransform socketLocation = GetMesh()->GetSocketTransform(spawnedEquipment->GetSocketName());
	spawnedEquipment->SetActorTransform(socketLocation);

	FAttachmentTransformRules attachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,true);
	spawnedEquipment->AttachToComponent(GetMesh(), attachRules, spawnedEquipment->GetSocketName());

}





//////////////////////////////////////////////////////////////////////////
// Input

void APersonalProj2_GP4Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APersonalProj2_GP4Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APersonalProj2_GP4Character::Look);
	
		// attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APersonalProj2_GP4Character::Attack);

		// rolling
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APersonalProj2_GP4Character::Roll);
		
		//blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, & APersonalProj2_GP4Character::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APersonalProj2_GP4Character::UnBlock);



	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APersonalProj2_GP4Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APersonalProj2_GP4Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	//gameplayWidgetHudClass = nullptr;
	//PlayerHud = nullptr;
	isDead = false;
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AController* controller = GetController();
	ControllerComp = Cast<APlayerController>(controller);
	isLockedOn = false;

	if(gameplayWidgetHudClass)
	{
		PlayerHud = CreateWidget<UPlayerUserWidget>(GetWorld(), gameplayWidgetHudClass);
		PlayerHud->AddToPlayerScreen();
	}
}
void APlayerCharacter::Tick(float DeltaTime)
{
	if (lockOnTarget)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),lockOnTarget->GetActorLocation());
		GetController()->SetControlRotation(Rotation);
	}
}

void APlayerCharacter::OnDead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	if (ControllerComp != nullptr)
	{
		DisableInput(ControllerComp);
	}

	if (isDead)
	{
		return;
	}
	PlayerHud->RemoveFromViewport();

	UDeathWidget* deathhud;
	deathhud = CreateWidget<UDeathWidget>(GetWorld(), deathWidgetClass);
	deathhud->AddToPlayerScreen();

	
	ControllerComp->SetShowMouseCursor(true);
	FInputModeGameAndUI inputMode;

	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	ControllerComp->SetInputMode(inputMode);
	isDead = true;
}

void APlayerCharacter::Attack()
{

	float MontageTime = PlayAnimMontage(AttackMontage);
	DisableInput(ControllerComp);
	FTimerHandle MontageTimerHandle;
	GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &APlayerCharacter::FinishedAnimation, MontageTime - AttackMovementTimer);

}
void APlayerCharacter::Roll()
{
	isRolling = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Ignore);
	float MontageTime = PlayAnimMontage(RollMontage);
	DisableInput(ControllerComp);
	FTimerHandle MontageTimerHandle;
	GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &APlayerCharacter::FinishedAnimation, MontageTime - RollMovementTimer);

}
void APlayerCharacter::Block()
{
	PlayAnimMontage(BlockMontage);
	isBlocking = true;
}
void APlayerCharacter::UnBlock()
{
	StopAnimMontage(BlockMontage);
	isBlocking = false;
}
void APlayerCharacter::LockOn()
{
	if (!isLockedOn)
	{
		isLockedOn = true;
		TArray<FHitResult> hitresults; 

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->SweepMultiByChannel(
			hitresults,
			GetActorLocation(),
			GetActorForwardVector() * 500 + GetActorLocation(),
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(300.0f),
			queryParams
			);

		if (bHit)
		{
			AActor* NextTarget = nullptr;
			float MinDistance = TNumericLimits<float>::Max();
			for (const FHitResult& Hit : hitresults)
			{
				AActor* HitActor = Hit.GetActor();
				if (HitActor->IsA(AEnemyCharacter::StaticClass()))
				{
					float DistanceToTarget = FVector::Distance(GetActorLocation(), HitActor->GetActorLocation());
					if (DistanceToTarget < MinDistance)
					{
						MinDistance = DistanceToTarget;
						NextTarget = HitActor;
					}
				}
			}

			lockOnTarget = NextTarget;

			//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *lockOnTarget->GetName());
		}		
	}
	else
	{
		lockOnTarget = nullptr;
		isLockedOn = false;
	}
}
UPlayerUserWidget* APlayerCharacter::GetHud()
{
	return PlayerHud;
}
void APlayerCharacter::UpdateHud()
{
	PlayerHud->SetHealth(health, MaxHealth);
}
bool APlayerCharacter::GetIsBlocking()
{
	return isBlocking;
}
bool APlayerCharacter::GetIsRolling()
{
	return isRolling;
}
void APlayerCharacter::FinishedAnimation()
{
	EnableInput(ControllerComp);

	if (isRolling)
	{
		isRolling = false;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);

		// rolling
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Roll);

		//blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &APlayerCharacter::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APlayerCharacter::UnBlock);

		//Lock On
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &APlayerCharacter::LockOn);


	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
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

void APlayerCharacter::Look(const FInputActionValue& Value)
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

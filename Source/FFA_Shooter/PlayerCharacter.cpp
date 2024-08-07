// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"



APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);

	// Set default values for input actions and mapping context
	MoveAction = nullptr;
	SprintAction = nullptr;
	JumpAction = nullptr;
	InputMappingContext = nullptr;

	// Initialize the walking and sprinting speeds
	BaseRunSpeed = 600.0f; // Normal run speed
	SprintSpeed = 1200.0f;  // Speed when sprinting

	// Initialize the sprinting state
	bIsSprinting = false; // Not sprinting initially

	// Initialize FOV variables
	NormalFOV = 90.0f; // Default values
	SprintFOV = 120.0f;
	AimingFOV = 60.0f;
	TargetFOV = NormalFOV;
	TransitionDurationFOV = 2.0f;
	ElapsedTimeTransitionFOV = 0.0f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Check if we have a valid player controller and subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the FOV if needed
	if (TransitionDurationFOV > 0.0f)
	{
		UpdateFOV(DeltaTime);
	}

	FString FOVString = FString::Printf(TEXT("FieldOfView: %f"), Camera->FieldOfView);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FOVString);



}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast to EnhancedInputComponent to use Enhanced Input System
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind input actions to functions
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
		}

	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// Implement movement logic
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::Jump()
{
	Super::Jump();
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();
	AddControllerYawInput(MouseMovement.X);
	AddControllerPitchInput(MouseMovement.Y);
}


// Start the FOV transition
void APlayerCharacter::StartFOVTransition(float NewFOV, float Duration)
{
	TargetFOV = NewFOV;
	TransitionDurationFOV = Duration;
	ElapsedTimeTransitionFOV = 0.0f; // Reset elapsed time
}

// Update the FOV gradually
void APlayerCharacter::UpdateFOV(float DeltaTime)
{
	// Calculate the progress of the transition
	ElapsedTimeTransitionFOV += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTimeTransitionFOV / TransitionDurationFOV, 0.0f, 1.0f);

	// Interpolate the FOV value
	float CurrentFOV = FMath::Lerp(Camera->FieldOfView, TargetFOV, Alpha);
	Camera->FieldOfView = CurrentFOV;

	// Stop transition if complete
	if (Alpha >= 1.0f)
	{
		TransitionDurationFOV = 0.0f; // Stop updating
	}
}

// Method to start sprinting
void APlayerCharacter::StartSprinting()
{
	bIsSprinting = true; // Set the sprinting state to true
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // Set character speed to sprint speed
	StartFOVTransition(SprintFOV, TransitionDurationFOV);
}

// Method to stop sprinting
void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false; // Set the sprinting state to false
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed; // Set character speed back to base walk speed
	StartFOVTransition(NormalFOV, TransitionDurationFOV);
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"  
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h" 



APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsInputEnabled = false;

	CurrentWeaponIndex = 0;

	// Create the SpringArm and attach it to the RootComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // Set the distance of the camera from the character
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; // We don't want the camera to rotate on its own


	// Set default values for input actions and mapping context
	MoveAction = nullptr;
	SprintAction = nullptr;
	JumpAction = nullptr;
	InputMappingContext = nullptr;

	// Initialize the walking and sprinting speeds
	BaseRunSpeed = 600.0f; // Normal run speed
	SprintSpeed = 1000.0f;  // Speed when sprinting

	// Initialize stamina variables
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaDrainRate = 25.0f;
	StaminaRegenerationRate = 30.0f;
	StaminaDrainDelay = 0.1f;


	// Initialize the sprinting state
	bIsSprinting = false; // Not sprinting initially

	// Initialize FOV variables
	NormalFOV = 90.0f; // Default values
	SprintFOV = 100.0f;
	AimingFOV = 60.0f;
	TargetFOV = NormalFOV;
	TransitionDurationFOV = 0.4f;
	CurrentTransitionDurationFOV = 0.0f;
	ElapsedTimeTransitionFOV = 0.0f;

	// Initialize aiming variables
	IsAiming = false;
	AimingFOV = 50.0f;

	// Initialize Shooting variables
	CurrentWeapon = nullptr;
	bIsFiringAutomatic = false;
	bIsAutomatic = false; 
	FireRate = 0.1f;
	LastFireTime = 0.0f;

	PlayerHUD = nullptr;



}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Obtener el PlayerController actual de manera segura
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Asegúrate de que el PlayerController es válido
	if (PlayerController)
	{
		// Comienza la sección de creación del HUD
		if (HUDClass)
		{
			// Crear y mostrar el widget HUD
			PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, HUDClass);
			if (PlayerHUD)
			{
				PlayerHUD->AddToViewport();
			}
		}

		// Comienza la sección de configuración de InputSubsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// Spawn all linked Weapons
	SpawnWeapons();
	EquipWeapon(0);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInputEnabled)
	{
		// Update the FOV if needed
		if (CurrentTransitionDurationFOV > 0.0f)
		{
			UpdateFOV(DeltaTime);
		}

		// Handle automatic firing
		if (bIsFiringAutomatic && bIsAutomatic)
		{
			HandleFiring(DeltaTime);
		}

		if (bIsSprinting)
		{
			CurrentStamina -= StaminaDrainRate * DeltaTime;
			CurrentStamina = FMath::Max(CurrentStamina, 0.0f);
			PlayerHUD->UpdateStamina(CurrentStamina, MaxStamina);

			if (CurrentStamina <= 0.0f)
			{
				StopSprinting();
			}
		}
		else
		{
			CurrentStamina += StaminaRegenerationRate * DeltaTime;
			CurrentStamina = FMath::Min(CurrentStamina, MaxStamina); 
			PlayerHUD->UpdateStamina(CurrentStamina, MaxStamina);
		}
	}
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
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
		}
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);
		}
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::StartShooting);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopShooting);
		}
		if (ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
		}
		if (Weapon1Action)
		{
			EnhancedInputComponent->BindAction(Weapon1Action, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeapon0);
		}
		if (Weapon2Action)
		{
			EnhancedInputComponent->BindAction(Weapon2Action, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeapon1);
		}
		if (Weapon3Action)
		{
			EnhancedInputComponent->BindAction(Weapon3Action, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeapon2);
		}

	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsInputEnabled)
	{
		// Implement movement logic
		FVector2D MovementVector = Value.Get<FVector2D>();
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}

}

void APlayerCharacter::Jump()
{
	if (bIsInputEnabled)
	{
		Super::Jump();
	}
}

void APlayerCharacter::StopJumping()
{
	if (bIsInputEnabled)
	{
		Super::StopJumping();
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();
	
	AddControllerYawInput(MouseMovement.X);
	AddControllerPitchInput(-MouseMovement.Y);
}


// Start the FOV transition
void APlayerCharacter::StartFOVTransition(float NewFOV, float Duration)
{
	TargetFOV = NewFOV;
	CurrentTransitionDurationFOV = Duration;
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
		CurrentTransitionDurationFOV = 0.0f; // Stop updating
	}
}

// Method to start sprinting
void APlayerCharacter::StartSprinting()
{
	if (bIsInputEnabled)
	{
		if (!IsAiming && CurrentStamina > 0.0f)
		{
			// Get the character's current velocity
			FVector Velocity = GetCharacterMovement()->Velocity;

			// Calculate the magnitude of the velocity vector
			float Speed = Velocity.Size();

			// Define a minimum speed threshold for sprinting
			const float MinimumSpeedThreshold = 1.0f; // Adjust this value as needed

			// Only start sprinting if the character is moving faster than the threshold
			if (Speed > MinimumSpeedThreshold)
			{
				bIsSprinting = true; // Set the sprinting state to true
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // Set character speed to sprint speed
				StartFOVTransition(SprintFOV, TransitionDurationFOV);
			}
		}
	}
}

// Method to stop sprinting
void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false; // Set the sprinting state to false
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed; // Set character speed back to base walk speed
	if (!IsAiming)
	{
		StartFOVTransition(NormalFOV, TransitionDurationFOV);
	}
}

void APlayerCharacter::StartAiming()
{
	if (bIsInputEnabled)
	{
		IsAiming = true;
		if (bIsSprinting)
		{
			StopSprinting();
		}
		StartFOVTransition(AimingFOV, TransitionDurationFOV);
	}
}

void APlayerCharacter::StopAiming()
{
	IsAiming = false;
	if (bIsSprinting)
	{
		StartFOVTransition(SprintFOV, TransitionDurationFOV);
	}
	else
	{
		StartFOVTransition(NormalFOV, TransitionDurationFOV);

	}
}


void APlayerCharacter::SpawnWeapons()
{
    for (const TSubclassOf<AWeaponBase>& WeaponClass : WeaponClasses)
	{
		if (WeaponClass)
		{
			AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

			if (NewWeapon)
			{
				NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
				NewWeapon->SetOwner(this);
				NewWeapon->SetActorHiddenInGame(true); 
				NewWeapon->SetPlayerHUD(PlayerHUD);
				WeaponInventory.Add(NewWeapon);
			}
		}
	}
}

void APlayerCharacter::EquipWeapon(int32 WeaponIndex)
{

	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		if (WeaponInventory.IsValidIndex(CurrentWeaponIndex))
		{
			// Hide current Gun
			WeaponInventory[CurrentWeaponIndex]->SetActorHiddenInGame(true);

			// Show new gun
			CurrentWeaponIndex = WeaponIndex;
			WeaponInventory[CurrentWeaponIndex]->SetActorHiddenInGame(false);
			
			CurrentWeapon = WeaponInventory[CurrentWeaponIndex];
			bIsAutomatic = CurrentWeapon->bIsAutomatic;
			FireRate = CurrentWeapon->FireRate;

			CurrentWeapon->UpdateHUD();
			UGameplayStatics::PlaySoundAtLocation(this, ChangeGunSound, GetActorLocation());

		}	
	}

}

void APlayerCharacter::EquipWeapon0()
{
	EquipWeapon(0);
}

void APlayerCharacter::EquipWeapon1()
{
	EquipWeapon(1);
}

void APlayerCharacter::EquipWeapon2()
{
	EquipWeapon(2);
}

void APlayerCharacter::StartShooting()
{
	if (bIsInputEnabled)
	{
		if (bIsAutomatic)
		{
			bIsFiringAutomatic = true;
		}
		else {
			if (CurrentWeapon)
			{
				CurrentWeapon->Fire();
			}
		}
	}
}

void APlayerCharacter::StopShooting()
{
	if (bIsAutomatic)
	{
		bIsFiringAutomatic = false;
	}
}

void APlayerCharacter::HandleFiring(float DeltaTime)
{
	if (CurrentWeapon && (GetWorld()->GetTimeSeconds() - LastFireTime >= FireRate))
	{
		CurrentWeapon->Fire();
		LastFireTime = GetWorld()->GetTimeSeconds(); // Update the time of the last shot
	}
}


void APlayerCharacter::Reload()
{
	if (bIsInputEnabled)
	{
		CurrentWeapon->Reload();
		CurrentWeapon->UpdateHUD();
	}

}

void APlayerCharacter::ChangeInputValidation(bool inputValidation)
{
	bIsInputEnabled = inputValidation;
	SetBlurVisibility(!inputValidation);
}

void APlayerCharacter::SetBlurVisibility(bool visible)
{
	if (PlayerHUD)
	{
		PlayerHUD->SetBackgroundBlurVisibility(visible);
	}
}

void APlayerCharacter::UpdateCountdownValue(int32 value)
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdateCountdown(value);
	}
}



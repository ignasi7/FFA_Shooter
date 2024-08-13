#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerCharacterAnimInstance.h"
#include "WeaponBase.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.generated.h"




UCLASS()
class FFA_SHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses; // Weapons available to spawn

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<AWeaponBase*> WeaponInventory; // Spawned Weapons

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
	int32 CurrentWeaponIndex;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EquipWeapon(int32 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SpawnWeapons();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -------------------------------------------------------------- Camera
	// Spring Arm Component
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	// -------------------------------------------------------------- Input Actions
	// Input action properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* Weapon1Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* Weapon2Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* Weapon3Action;

	// Input mapping context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Effects")
	USoundBase* ChangeGunSound;

	// -------------------------------------------------------------- STAMINA

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenerationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainDelay;


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool bIsInputEnabled;
	void ChangeInputValidation(bool inputValidation);

	// HUD public functions
	void SetBlurVisibility(bool visible);
	void UpdateCountdownValue(int32 value);
	void UpdateScore(int32 value);

	// Input basic movement functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();

	// Methods to manage FOV
	void StartFOVTransition(float NewFOV, float Duration);
	void UpdateFOV(float DeltaTime);

	// Methods to start and stop sprinting
	void StartSprinting();
	void StopSprinting();

	// Methods to start and stop aiming
	void StartAiming();
	void StopAiming();

	// Methods to manage shooting
	void StartShooting();
	void StopShooting();

	void Reload();

	void EquipWeapon0();
	void EquipWeapon1();
	void EquipWeapon2();



private:

	// -------------------------------------------------------------- Movement

	// The base walking speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseRunSpeed;

	// The sprinting speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	// Indicates whether the character is currently sprinting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	

	// -------------------------------------------------------------- FOV
	// Normal FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NormalFOV;

	// Sprint FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintFOV;

	// Aiming FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AimingFOV;

	// Target FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TargetFOV;

	// FOV transition Duration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TransitionDurationFOV;

	// Current FOV transition Duration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CurrentTransitionDurationFOV;

	// FOV Transition Elapsed Time
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ElapsedTimeTransitionFOV;

	// Is Aiming Boleean
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool IsAiming;


	// -------------------------------------------------------------- Shooting
	// 
	// Variable to keep track of the current weapon
	UPROPERTY()
	AWeaponBase* CurrentWeapon;

	// Is firing or not an automatic gun
	bool bIsFiringAutomatic; 
	
	// Is the weapon automatic or not
	bool bIsAutomatic; 
	
	// Fire rate of the weapon
	float FireRate; 
	
	// Time of the last shot
	float LastFireTime; 

	// Handle firing logic
	void HandleFiring(float DeltaTime);


};

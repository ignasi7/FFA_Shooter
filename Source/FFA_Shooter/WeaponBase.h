// WeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Pistol UMETA(DisplayName = "Pistol"),
    SMG UMETA(DisplayName = "SMG"),
    Shotgun UMETA(DisplayName = "Shotgun")
};

UCLASS()
class FFA_SHOOTER_API AWeaponBase : public AActor
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // HUD reference
    UPROPERTY()
    UPlayerHUD* PlayerHUD;

public:

    // Constructor
    AWeaponBase();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamagePerShot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 MagazineSize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 MaxMagazines;

    UPROPERTY(BlueprintReadWrite, Category = "Weapon")
    int32 CurrentAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float ReloadTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bIsAutomatic;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Effects")
    USoundBase* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Effects")
    USoundBase* EmptyGunSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* MuzzleFlashComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ImpactFlashComponent;

    UPROPERTY()
    APlayerController* PlayerCharacterController;


    // Methods
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Reload();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetPlayerHUD(UPlayerHUD* NewHUD);
};


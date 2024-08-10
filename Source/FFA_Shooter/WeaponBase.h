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

public:
    // Constructor
    AWeaponBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* MuzzleFlashComponent;

   // UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Effects")
   // UParticleSystem* ImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Effects")
    FName MuzzleSocketName;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Reload();
};


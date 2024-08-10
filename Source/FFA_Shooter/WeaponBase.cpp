#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"

// Constructor
AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentAmmo = MagazineSize; // Initialize ammo to a full magazine
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeaponBase::Fire()
{
    UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hola"));


    /*
    if (CurrentAmmo > 0)
    {
        --CurrentAmmo;
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetRootComponent(), MuzzleSocketName);
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

        // Implement shooting logic (raycast, damage application, etc.)
    }
    else
    {
        // Play empty click sound or reload prompt
    }*/
}

void AWeaponBase::Reload()
{
    if (CurrentAmmo < MagazineSize)
    {
        // Reload logic
        const int32 AmmoToReload = MagazineSize - CurrentAmmo;
        const int32 AmmoAvailable = MaxMagazines * MagazineSize - CurrentAmmo; // Simplified calculation

        const int32 AmmoToAdd = FMath::Min(AmmoToReload, AmmoAvailable);

        if (AmmoToAdd > 0)
        {
            CurrentAmmo += AmmoToAdd;

            // Play reload sound, start reload animation, etc.
            // Consider using a timer to simulate reload time if necessary
        }
    }
}

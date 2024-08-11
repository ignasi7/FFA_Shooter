#include "WeaponBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "DrawDebugHelpers.h"


// Constructor
AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    MuzzleFlashComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
    MuzzleFlashComponent->SetupAttachment(MeshComponent, TEXT("Muzzle"));
    MuzzleFlashComponent->bAutoActivate = false;

    ImpactFlashComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ImpactFlash"));
    ImpactFlashComponent->bAutoActivate = false;

    PlayerHUD = nullptr;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmmo = MagazineSize;

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerCharacterController = PlayerController;
    }

    if (PlayerHUD)
    {
        PlayerHUD->UpdateAmmo(CurrentAmmo, MagazineSize);

    }
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeaponBase::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("FIRE OUT")); 

    // Log the weapon stats
    UE_LOG(LogTemp, Warning, TEXT("Weapon Stats:"));
    UE_LOG(LogTemp, Warning, TEXT("Damage Per Shot: %f"), DamagePerShot);
    UE_LOG(LogTemp, Warning, TEXT("Magazine Size: %d"), MagazineSize);
    UE_LOG(LogTemp, Warning, TEXT("Max Magazines: %d"), MaxMagazines);
    UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %d"), CurrentAmmo);
    UE_LOG(LogTemp, Warning, TEXT("Fire Rate: %f"), FireRate);
    UE_LOG(LogTemp, Warning, TEXT("Reload Time: %f"), ReloadTime);
    UE_LOG(LogTemp, Warning, TEXT("Weapon Type: %d"), static_cast<int32>(WeaponType)); // Assuming EWeaponType is an enum


    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("FIRE IN"));

        --CurrentAmmo;

        if (PlayerHUD)
        {
            PlayerHUD->UpdateAmmo(CurrentAmmo, MagazineSize);
        }
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        MuzzleFlashComponent->ActivateSystem(true);

        // Implement shooting logic (raycast, damage application, etc.)
        // Step 1: Get the start location from the muzzle socket
        FVector MuzzleLocation = MeshComponent->GetSocketLocation("Muzzle");

        // Step 2: Get the forward vector for the direction of the trace
        FVector CameraLocation;
        FRotator CameraRotation;
        PlayerCharacterController->GetPlayerViewPoint(CameraLocation, CameraRotation);
        FVector TraceDirection = CameraRotation.Vector();

        // Step 3: Define the endpoint of the trace
        FVector EndLocation = MuzzleLocation + (TraceDirection * 10000.f); // 10000 units in the direction of the trace

        // Step 4: Perform the line trace
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);
        CollisionParams.AddIgnoredActor(GetOwner());

        if (GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndLocation, ECC_Visibility, CollisionParams))
        {
            // If we hit something, update the end location to the impact point
            EndLocation = HitResult.ImpactPoint;
            ImpactFlashComponent->SetWorldLocation(EndLocation);
            ImpactFlashComponent->ActivateSystem(true);

            // You can do additional logic here if you need to handle what happens on hit
        }

        // Step 5: Draw the debug line to visualize the trace
        DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Red, false, 0.1f, 0, 1.0f); // Last two parameters control thickness
    }
    else
    {
        UGameplayStatics::PlaySoundAtLocation(this, EmptyGunSound, GetActorLocation());
    }
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


void AWeaponBase::SetPlayerHUD(UPlayerHUD* NewHUD)
{
    PlayerHUD = NewHUD;
}

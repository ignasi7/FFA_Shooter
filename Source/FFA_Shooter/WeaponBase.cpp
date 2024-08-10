#include "WeaponBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Constructor
AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentAmmo = MagazineSize; // Initialize ammo to a full magazine

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    MuzzleFlashComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
    MuzzleFlashComponent->SetupAttachment(MeshComponent, MuzzleSocketName);
    MuzzleFlashComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerCharacterController = PlayerController;
    }
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


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

        // You can do additional logic here if you need to handle what happens on hit
    }

    // Step 5: Draw the debug line to visualize the trace
    DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Red, false, 0.1f, 0, 1.0f); // Last two parameters control thickness
}

void AWeaponBase::Fire()
{
    UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    MuzzleFlashComponent->ActivateSystem(true);

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

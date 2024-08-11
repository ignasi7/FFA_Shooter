#include "WeaponBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "TimerManager.h"
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

    IsReloading = false;
    CanFireShotgun = true;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmmo = MagazineSize;
    RemainingMagazines = MaxMagazines;

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerCharacterController = PlayerController;
    }

    if (PlayerHUD)
    {
        PlayerHUD->UpdateAmmo(CurrentAmmo, MagazineSize);
        PlayerHUD->UpdateMagazines(RemainingMagazines);
    }
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeaponBase::Fire()
{
    if (IsReloading) {
        return;
    }
    if (CurrentAmmo > 0) 
    {
        if(WeaponType == EWeaponType::Pistol || WeaponType == EWeaponType::SMG || (WeaponType == EWeaponType::Shotgun && CanFireShotgun))
        {
            --CurrentAmmo;

            if (PlayerHUD)
            {
                UpdateHUD();
            }
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
            MuzzleFlashComponent->ActivateSystem(true);

            if (WeaponType == EWeaponType::Shotgun)
            {
                CanFireShotgun = false;
                float FireSoundDuration = FireSound ? FireSound->GetDuration() : 1.0f;
                GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::ResetFireAbility, FireSoundDuration, false);
            }

            // Implement shooting logic
            FVector CameraLocation;
            FRotator CameraRotation;
            PlayerCharacterController->GetPlayerViewPoint(CameraLocation, CameraRotation);
            FVector TraceDirection = CameraRotation.Vector();

            // Define the endpoint of the trace
            FVector EndLocation = CameraLocation + (TraceDirection * 10000.f); 

            // Perform the line trace
            FHitResult HitResult;
            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(this);
            CollisionParams.AddIgnoredActor(GetOwner());

            if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECC_Visibility, CollisionParams))
            {
                // If we hit something, update the end location to the impact point
                EndLocation = HitResult.ImpactPoint;
                ImpactFlashComponent->SetWorldLocation(EndLocation);
                ImpactFlashComponent->ActivateSystem(true);
            }

           // DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Red, false, 0.1f, 0, 1.0f); // Last two parameters control thickness
        }
    }
    else
    {
        UGameplayStatics::PlaySoundAtLocation(this, EmptyGunSound, GetActorLocation());
    }
}

void AWeaponBase::ResetFireAbility()
{
    CanFireShotgun = true;
}

void AWeaponBase::Reload()
{
    if (IsReloading)
    {
        return;
    }

    if (CurrentAmmo < MagazineSize && RemainingMagazines > 0)
    {
        IsReloading = true; // Set the reloading flag

        const int32 AmmoToReload = MagazineSize - CurrentAmmo;
        const int32 AmmoAvailable = RemainingMagazines * MagazineSize;

        const int32 AmmoToAdd = FMath::Min(AmmoToReload, AmmoAvailable);

        if (AmmoToAdd > 0)
        {
            // Play reload sound
            if (ReloadSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
            }

            // Schedule a timer to simulate reload time
            float ReloadDuration = ReloadSound ? ReloadSound->GetDuration() : 1.0f; // Default to 1 second if no sound
            GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::FinishReloading, ReloadDuration, false);
        }
    }
    else
    {
        UGameplayStatics::PlaySoundAtLocation(this, ErrorSound, GetActorLocation());
    }
}

void AWeaponBase::FinishReloading()
{
    // Reset reloading flag
    IsReloading = false;

    const int32 AmmoToReload = MagazineSize - CurrentAmmo;
    const int32 AmmoAvailable = RemainingMagazines * MagazineSize;

    const int32 AmmoToAdd = FMath::Min(AmmoToReload, AmmoAvailable);

    if (AmmoToAdd > 0)
    {
        CurrentAmmo += AmmoToAdd;
        RemainingMagazines -= 1; 
    }

    if (PlayerHUD)
    {
        UpdateHUD();
    }
}



void AWeaponBase::SetPlayerHUD(UPlayerHUD* NewHUD)
{
    PlayerHUD = NewHUD;
}

void AWeaponBase::UpdateHUD()
{
    if (PlayerHUD)
    {
        FName WeaponName = GetWeaponName();
        PlayerHUD->UpdateGun(WeaponName);        
        PlayerHUD->UpdateAmmo(CurrentAmmo, MagazineSize);
        PlayerHUD->UpdateMagazines(RemainingMagazines);
    }
}

FName AWeaponBase::GetWeaponName() const
{
    switch (WeaponType)
    {
    case EWeaponType::Pistol:
        return FName("Pistol");
    case EWeaponType::SMG:
        return FName("SMG");
    case EWeaponType::Shotgun:
        return FName("Shotgun");
    default:
        return FName("Unknown");
    }
}

#include "AIEnemy.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h" 


AAIEnemy::AAIEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    CurrentDestination = GetActorLocation();
    MovementRadius = 10000.0f;
}

void AAIEnemy::BeginPlay()
{
    Super::BeginPlay();

    PlayAnimation(0);

    SetNextDestination();
}

void AAIEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentDestination.IsZero() || FVector::Dist(GetActorLocation(), CurrentDestination) < 100.0f)
    {
        SetNextDestination();
    }
}

void AAIEnemy::SetNextDestination()
{
    // Get AI controller
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        // Get navigation system
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys)
        {
            // Define a random point within a radius around the current location
            FVector Origin = GetActorLocation();
            FNavLocation RandomLocation;

            // Try to find a valid random location within a navigable radius
            if (NavSys->GetRandomPointInNavigableRadius(Origin, MovementRadius, RandomLocation, nullptr))
            {
                // Set the new destination
                CurrentDestination = RandomLocation.Location;
                AIController->MoveToLocation(CurrentDestination);

                //DrawDebugSphere(GetWorld(), CurrentDestination, 50.0f, 12, FColor::Red, true, -1.0f, 0, 2.0f);

                PlayAnimation(1);
            }
        }
    }
}

void AAIEnemy::PlayAnimation(int animation)
{
    // 0 for idle
    // 1 for running
    if (Animations.Num() > 0)
    {
        USkeletalMeshComponent* MeshComponent = GetMesh();
        if (MeshComponent)
        {
            MeshComponent->PlayAnimation(Animations[animation], true);
        }
    }
}

void AAIEnemy::ReduceHealth(float damage)
{
    if (CurrentHealth > 0)
    {
        CurrentHealth -= damage;
        /*FString HealthMessage = FString::Printf(TEXT("Current Health: %f"), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HealthMessage);*/
        if (CurrentHealth <= 0)
        {
            Die();
        }
    }
}


void AAIEnemy::Die()
{
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

    AController* AIController = GetController();
    if (AIController)
    {
        AIController->UnPossess();
    }

    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->IncreaseScore();
    }

    FTimerHandle RespawnTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AAIEnemy::Respawn, 3.0f, false);
}

void AAIEnemy::Respawn()
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSys)
    {
        FNavLocation RandomLocation;
        FVector Origin = GetActorLocation();

        if (NavSys->GetRandomPointInNavigableRadius(Origin, MovementRadius, RandomLocation, nullptr))
        {
            FVector SpawnLocation = RandomLocation.Location;
            SpawnLocation.Z = 90.0f;
            //DrawDebugSphere(GetWorld(), RandomLocation.Location, 50.0f, 12, FColor::Green, false, 10.0f);
            AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
            if (GameMode)
            {
                GameMode->SpawnEnemy(SpawnLocation);
            }
        }
    }
    Destroy();
}


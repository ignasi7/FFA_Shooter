// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h" // Para usar GetCharacterMovement
#include "DrawDebugHelpers.h" 


// Sets default values
AAIEnemy::AAIEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set default values for health
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    CurrentDestination = GetActorLocation();
    MovementRadius = 10000.0f;


}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
    Super::BeginPlay();

    PlayAnimation(0);

    SetNextDestination();
}

void AAIEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Verifica si el enemigo ha llegado al destino actual
    if (CurrentDestination.IsZero() || FVector::Dist(GetActorLocation(), CurrentDestination) < 100.0f)
    {
        // Si el destino actual está cerca, selecciona un nuevo destino
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

                DrawDebugSphere(GetWorld(), CurrentDestination, 50.0f, 12, FColor::Red, true, -1.0f, 0, 2.0f);

                PlayAnimation(1);
            }
        }
    }
}

void AAIEnemy::PlayAnimation(int animation)
{
    // 0 for idle
    // 1 for running
    // 2 for sprint
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

    CurrentHealth -= damage;
    FString HealthMessage = FString::Printf(TEXT("Current Health: %f"), CurrentHealth);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HealthMessage);
    if (CurrentHealth <= 0)
    {
        Die();
    }
}


void AAIEnemy::Die()
{
    FString DeathMessage = FString::Printf(TEXT("Enemy %s has died"), *GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);

    // Desactiva el control del Character Movement Component
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    // Activa el ragdoll
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

    // Desactiva el control por el AI Controller (si lo hay)
    AController* AIController = GetController();
    if (AIController)
    {
        AIController->UnPossess();
    }
}



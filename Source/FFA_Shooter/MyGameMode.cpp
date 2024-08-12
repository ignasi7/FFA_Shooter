// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "PlayerCharacter.h"
#include "AIEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Constructor de AMyGameMode
AMyGameMode::AMyGameMode()
{
    StartTimeDelay = 10;
    Score = 0;
    CurrentTimeDelay = StartTimeDelay;
    PlayerCharacterInstance = nullptr;
}


void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerCharacterClass, FoundActors);

    if (FoundActors.Num() > 0)
    {
        PlayerCharacterInstance = Cast<APlayerCharacter>(FoundActors[0]);
    }

	EnableCharacterInput(false);

	//GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &AMyGameMode::UnlockGame, StartTimeDelay, false);
    GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &AMyGameMode::ManageCountdown, 1.0f, true);


}

void AMyGameMode::UnlockGame()
{
	// Unblock characters
	EnableCharacterInput(true);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("unlock game"));

}

void AMyGameMode::ManageCountdown() {
    PlayerCharacterInstance->UpdateCountdownValue(CurrentTimeDelay);
    --CurrentTimeDelay;
    if (CurrentTimeDelay < 0)
    {
        EnableCharacterInput(true);
        if (PlayerCharacterInstance)
        {
            PlayerCharacterInstance->SetBlurVisibility(false);
        }
    }
}


void AMyGameMode::EnableCharacterInput(bool bBlock)
{
    if (PlayerCharacterInstance)
    {
        PlayerCharacterInstance->ChangeInputValidation(bBlock);
    }
}

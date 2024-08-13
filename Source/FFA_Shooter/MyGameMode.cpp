#include "MyGameMode.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h" 
#include "Engine/World.h"



// Constructor de AMyGameMode
AMyGameMode::AMyGameMode()
{
    StartTimeDelay = 10;
    MaxGameTime = 60;
    RemainingTime = MaxGameTime;
    GameStarted = false;
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
        if (PlayerCharacterInstance)
        {
            PlayerCharacterInstance->UpdateScore(Score);
            PlayerCharacterInstance->SetBlurEndVisibility(false, Score);

        }
    }

	EnableCharacterInput(false);

    GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &AMyGameMode::ManageCountdown, 1.0f, true);


}


void AMyGameMode::UnlockGame()
{
	// Unblock characters
    GameStarted = true;
	EnableCharacterInput(true);
}

void AMyGameMode::ManageCountdown() {

    PlayerCharacterInstance->UpdateCountdownValue(CurrentTimeDelay);
    --CurrentTimeDelay;
    if (CurrentTimeDelay < 0)
    {
        UnlockGame();
        if (PlayerCharacterInstance)
        {
            PlayerCharacterInstance->SetBlurVisibility(false);
        }
    }

    if (GameStarted)
    {
        PlayerCharacterInstance->UpdateTimer(RemainingTime);
        --RemainingTime;
        if (RemainingTime <= 0)
        {
            if (PlayerCharacterInstance)
            {
                PlayerCharacterInstance->ChangeInputValidation(false);
                PlayerCharacterInstance->SetBlurEndVisibility(true, Score);
            }
            GameStarted = false;
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

int AMyGameMode::GetScore()
{
    return Score;
}

void AMyGameMode::IncreaseScore()
{
    Score++;
    if (PlayerCharacterInstance)
    {
        PlayerCharacterInstance->UpdateScore(Score);
    }
    /*FString ScoreMessage = FString::Printf(TEXT("Current Score: %d"), Score);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, ScoreMessage);*/
}

void AMyGameMode::SpawnEnemy(FVector position)
{
    if (PlayerCharacterInstance)
    {
        PlayerCharacterInstance->SpawnEnemy(position);
    }
}


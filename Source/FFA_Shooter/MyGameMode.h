#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include "GameFramework/GameMode.h"
#include "PlayerCharacter.h"
#include "MyGameMode.generated.h"


UCLASS()
class FFA_SHOOTER_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Constructor
	AMyGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer handle for the delay
	FTimerHandle StartGameTimerHandle;

	void EnableCharacterInput(bool bBlock);

	// Function to unlock the game after a delay
	void UnlockGame();

	void ManageCountdown();


	int32 StartTimeDelay;
	int32 CurrentTimeDelay;

	int32 Score;

	int32 MaxGameTime;
	int32 RemainingTime;

	bool GameStarted;

	APlayerCharacter* PlayerCharacterInstance;


public:
	// Reference to the PlayerCharacter class (assignable in Blueprint)
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSubclassOf<APlayerCharacter> PlayerCharacterClass;

	int GetScore();
	void IncreaseScore();	
	void SpawnEnemy(FVector position);

};

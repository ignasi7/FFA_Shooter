// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class FFA_SHOOTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateGun(FName GunName);
    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
    void UpdateMagazines(int32 RemainingMags);
    void UpdateHealth(int32 CurrentHealth, int32 MaxHealth);
    void UpdateStamina(int32 CurrentStamina, int32 MaxStamina);
    void UpdateCountdown(int32 countdown);
    void SetBackgroundBlurVisibility(bool visible);

protected:

    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmmoText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* GunText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MagsText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* StaminaText;

    UPROPERTY(meta = (BindWidget))
    UBackgroundBlur* BackgroundBlurStart;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CountdownText;
	
};

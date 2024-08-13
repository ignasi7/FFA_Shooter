#include "PlayerHUD.h"



void UPlayerHUD::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("Ammo: %d / %d"), CurrentAmmo, MaxAmmo)));
    }
}

void UPlayerHUD::UpdateGun(FName GunName)
{
    if (GunText)
    {
        FString GunNameString = GunName.ToString();
        GunText->SetText(FText::FromString(FString::Printf(TEXT("GUN: %s"), *GunNameString)));
    }
}

void UPlayerHUD::UpdateMagazines(int32 RemainingMagazines)
{
    if (MagsText)
    {
        MagsText->SetText(FText::FromString(FString::Printf(TEXT("Mags: %d"), RemainingMagazines)));
    }
}

void UPlayerHUD::UpdateHealth(int32 CurrentHealth, int32 MaxHealth)
{
    if (HealthText)
    {
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("Health: %d / %d"), CurrentHealth, MaxHealth)));
    }
}

void UPlayerHUD::UpdateStamina(int32 CurrentStamina, int32 MaxStamina)
{
    if (StaminaText)
    {
        StaminaText->SetText(FText::FromString(FString::Printf(TEXT("Stamina: %d / %d"), CurrentStamina, MaxStamina)));
    }
}

void UPlayerHUD::SetBackgroundBlurVisibility(bool visible)
{
    if (BackgroundBlurStart)
    {
        if (visible)
        {
            BackgroundBlurStart->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            BackgroundBlurStart->SetVisibility(ESlateVisibility::Hidden);

        }
    }
}

void UPlayerHUD::UpdateCountdown(int32 countdown)
{
    if (CountdownText)
    {
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), countdown)));
    }
}

void UPlayerHUD::UpdateScore(int32 score)
{
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("SCORE: %d"), score)));
    }
}

void UPlayerHUD::UpdateReimainingTime(int32 time)
{
    if(RemainingTimetext)
    {
        RemainingTimetext->SetText(FText::FromString(FString::Printf(TEXT("Remaining Time: %d"), time)));
    }
}


void UPlayerHUD::SetBackgroundBlurEndVisibility(bool visible, int32 score)
{
    if (BackgroundBlurEnd)
    {
        if (visible)
        {
            BackgroundBlurEnd->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            BackgroundBlurEnd->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
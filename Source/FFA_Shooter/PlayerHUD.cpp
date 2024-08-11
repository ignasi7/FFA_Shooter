// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"



void UPlayerHUD::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("Ammo: %d / %d"), CurrentAmmo, MaxAmmo)));
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
        StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentStamina, MaxStamina)));
    }
}

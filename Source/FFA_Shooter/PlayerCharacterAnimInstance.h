#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

UCLASS()
class FFA_SHOOTER_API UPlayerCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    FRotator SpineRotation; 

    UFUNCTION(BlueprintCallable, Category = "Custom")
    void SetSpineRotation(const FRotator& NewSpineRotation);
};

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerEnemy.generated.h"

UCLASS()
class FFA_SHOOTER_API AAIControllerEnemy : public AAIController
{
    GENERATED_BODY()

public:
    AAIControllerEnemy();

protected:
    virtual void BeginPlay() override;
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HDGameplayAbility.h"
#include "HDEnemyTeleport.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDEnemyTeleport : public UHDGameplayAbility
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	FVector GetTeleportLocation() const;
};

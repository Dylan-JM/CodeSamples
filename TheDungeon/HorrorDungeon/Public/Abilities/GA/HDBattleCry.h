// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDProjectileSpell.h"
#include "HDBattleCry.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDBattleCry : public UHDProjectileSpell
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnExplosions(FVector SpawnLocation);
};

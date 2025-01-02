// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HDGameplayAbility.h"
#include "HDBlock.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDBlock : public UHDGameplayAbility
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void SetBlockChance(float InBlockChance);
};

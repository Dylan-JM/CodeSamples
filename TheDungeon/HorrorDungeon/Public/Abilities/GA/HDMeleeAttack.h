// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDDamageGameplayAbility.h"
#include "HDMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDMeleeAttack : public UHDDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Melee")
	void SetDamage();
};

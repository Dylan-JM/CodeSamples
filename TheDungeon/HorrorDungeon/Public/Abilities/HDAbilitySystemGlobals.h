// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "HDAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};

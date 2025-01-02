// Copyright DM


#include "Abilities/HDAbilitySystemGlobals.h"
#include "HDAbilityTypes.h"

FGameplayEffectContext* UHDAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FHDGameplayEffectContext();
}

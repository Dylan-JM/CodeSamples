// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HDGameplayAbility.h"
#include "HDPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDPassiveAbility : public UHDGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};

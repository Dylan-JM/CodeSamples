// Copyright DM


#include "Abilities/GA/HDPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/HDAbilitySystemComponent.h"

void UHDPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		HDASC->DeactivatePassiveAbility.AddUObject(this, &UHDPassiveAbility::ReceiveDeactivate);
	}
}

void UHDPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

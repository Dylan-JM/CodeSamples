// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HDGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Abilities/HDAttributeSet.h"
#include "Interfaces/CombatInterface.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "Weapons/BaseWeapon.h"

UHDGameplayAbility::UHDGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UHDGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ICombatInterface::Execute_SetIsAttacking(GetAvatarActorFromActorInfo(), true);
}

void UHDGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	ICombatInterface::Execute_SetIsAttacking(GetAvatarActorFromActorInfo(), false);
}

FString UHDGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UHDGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UHDGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until next Level: %d</>"), Level);
}

void UHDGameplayAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
}

bool UHDGameplayAbility::IsCorrectWeapon(TSubclassOf<ABaseWeapon> Weapon)
{
	if (ICombatInterface::Execute_GetEquippedWeapon1(OwnerCharacter) == nullptr) return false;
	return ICombatInterface::Execute_GetEquippedWeapon1(OwnerCharacter)->IsA(Weapon);
}

float UHDGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UHDAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}	
	}
	return ManaCost;
}

float UHDGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

FTaggedMontage UHDGameplayAbility::GetTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages, FGameplayTag MontageTag) const
{
	for (FTaggedMontage TargetMontage : TaggedMontages)
	{
		if (TargetMontage.MontageTag == MontageTag)
		{
			return TargetMontage;
		}
	}
	return FTaggedMontage();
}

FTaggedMontage UHDGameplayAbility::GetRandomMontageWithTag(const TArray<FTaggedMontage>& TaggedMontages,
	FGameplayTag MontageTag) const
{
	TArray<FTaggedMontage> MontagesWithTag;
	for (FTaggedMontage TargetMontage : TaggedMontages)
	{
		if (TargetMontage.MontageTag == MontageTag)
		{
			MontagesWithTag.Add(TargetMontage);
		}
	}
	if (MontagesWithTag.Num()>0)
	{
		const int32 Selection = FMath::RandRange(0, MontagesWithTag.Num()-1);
		return MontagesWithTag[Selection];
	}
	return FTaggedMontage();
}

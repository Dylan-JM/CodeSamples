// Copyright DM


#include "Abilities/GA/HDMeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HDGameplayTags.h"
#include "Abilities/HDAttributeSet.h"
#include "Enemy/BaseEnemy.h"
#include "Interfaces/EnemyInterface.h"
#include "PlayerCharacter/BaseCharacter.h"
#include "Weapons/BaseWeapon.h"

void UHDMeleeAttack::SetDamage()
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	const FHDGameplayTags GameplayTags = FHDGameplayTags::Get();

	float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(BaseCharacter->GetAttributeSet());
	if (HDAS && BaseCharacter->EquippedWeapon1)
	{
		ScaledDamage += (HDAS->GetStrength() * BaseCharacter->EquippedWeapon1->MeleeDamageMultiplier);
	}
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);
	
	if (BaseCharacter && BaseCharacter->EquippedWeapon1)
	{
		if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(BaseCharacter->EquippedWeapon1))
		{
			Weapon->DamageEffectSpecHandle = SpecHandle;
			if (BaseCharacter->IsA(ABaseEnemy::StaticClass()))
			{
				Weapon->DebuffParams = MakeDamageEffectParamsFromClassDefaults(IEnemyInterface::Execute_GetCombatTarget(BaseCharacter));
			}
		}
	}
	if (BaseCharacter && BaseCharacter->EquippedOffhandItem)
	{
		if (ABaseWeapon* Weapon2 = Cast<ABaseWeapon>(BaseCharacter->EquippedOffhandItem))
		{
			Weapon2->DamageEffectSpecHandle = SpecHandle;
		}
	}
}

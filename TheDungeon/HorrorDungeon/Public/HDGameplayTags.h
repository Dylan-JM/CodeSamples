// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/* HDGameplayTags
 *
 * Singleton containing native Gameplay Tags
 * 
*/

struct FHDGameplayTags
{

public:

	static const FHDGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;

	FGameplayTag Attributes_Meta_IncomingXP;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_SHIFT;
	FGameplayTag InputTag_CTRL;
	FGameplayTag InputTag_SPACE;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Melee;
	FGameplayTag InputTag_Block;

	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	FGameplayTag Buff_Shield;

	
	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Enemy_Melee;
	FGameplayTag Abilities_Enemy_Teleport;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Fire_Flare;
	FGameplayTag Abilities_Fire_Barrage;
	FGameplayTag Abilities_Fire_Arrow;
	
	FGameplayTag Abilities_Lightning_Electrocute;
	
	FGameplayTag Abilities_Arcane_ArcaneShards;
	FGameplayTag Abilities_Arcane_BattleCry;

	FGameplayTag Abilities_Physical_DefaultMelee;
	FGameplayTag Abilities_Physical_AbilityMelee;
	FGameplayTag Abilities_Physical_RadiantStrike;
	FGameplayTag Abilities_Physical_JumpAttack;
	FGameplayTag Abilities_Physical_SpinAttack;
	FGameplayTag Abilities_Physical_GroundSmash;
	FGameplayTag Abilities_Physical_GroundSmashCombo;
	FGameplayTag Abilities_Physical_ThrowAxe;
	FGameplayTag Abilities_Physical_QuickAttack;
	FGameplayTag Abilities_Physical_Arrow;

	FGameplayTag Abilities_Movement_Jump;
	FGameplayTag Abilities_Movement_Step;
	FGameplayTag Abilities_Movement_Sprint;
	FGameplayTag Abilities_Movement_Teleport;
	FGameplayTag Abilities_Movement_Platform;
	
	FGameplayTag Abilities_Defence_Block; 
	FGameplayTag Abilities_Defence_Heal; 
	FGameplayTag Abilities_Defence_Revive; 
	FGameplayTag Abilities_Defence_SelfRevive; 
	
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_Arrow;
	FGameplayTag Cooldown_Physical_Arrow;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Montage_Attack_End;
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Attack_5;
	FGameplayTag Montage_Attack_6;
	FGameplayTag Montage_Attack_RadiantStrike;
	FGameplayTag Montage_Attack_RadiantStrikeEnd;
	FGameplayTag Montage_Attack_DisableCollision;
	FGameplayTag Montage_Attack_Spin;
	FGameplayTag Montage_Defence_Heal;
	FGameplayTag Montage_Jump;

	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	FGameplayTag Effect_HitReact;
	FGameplayTag Effect_StaminaRegen;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;


	FGameplayTag GameplayCue_FireBlast;
	
private:
	static FHDGameplayTags GameplayTags;
	
};

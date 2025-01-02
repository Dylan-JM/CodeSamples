// Copyright DM


#include "HDGameplayTags.h"
#include "GameplayTagsManager.h"

FHDGameplayTags FHDGameplayTags::GameplayTags;

void FHDGameplayTags::InitializeNativeGameplayTags()
{
		/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
		);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
		);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
		);

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
		);

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
		);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
		);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
		);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
		);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
		);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
		);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
		);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
		);

	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegeneration"),
	FString("Amount of Stamina regenerated every 1 second")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
		);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
		);

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
	FString("Maximum amount of Stamina obtainable")
	);
	
	/*
	* Input Tags
	*/

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
		);

	GameplayTags.InputTag_SHIFT = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SHIFT"),
FString("Input Tag for Left Shift Key")
	);
	
	GameplayTags.InputTag_CTRL = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.CTRL"),
	FString("Input Tag for Left Control Key")
	);

	GameplayTags.InputTag_SPACE = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SPACE"),
FString("Input Tag for SPACE Key")
	);
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 Key")
		);
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 Key")
		);
		
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 Key")
		);
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 Key")
		);

	GameplayTags.InputTag_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Melee"),
	FString("Input Tag for Melee Key")
	);

	GameplayTags.InputTag_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Block"),
FString("Input Tag for Block Key")
	);

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
	FString("Input Tag for Passive 1")
	);

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
FString("Input Tag for Passive 2")
	);

	/*
	 * Damage Types
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
	FString("Damage")
	);

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
	FString("Fire Damage Type")
	);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
	FString("Lightning Damage Type")
	);
	
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);
	
	/*
	 * Resistances
	 */
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damage")
		);
	
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage")
		);
	
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
		);
	
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage")
		);

	/*
	 * Debuffs
	*/
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Arcane"),
		FString("Debuff for Arcane damage")
		);
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire damage")
		);
	
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Lightning damage")
		);
	
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Physical"),
		FString("Debuff for Physical damage")
		);

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Chance"),
	FString("Debuff Chance")
	);
	
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);
	

	/*
	* Meta Attributes
	*/
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("IncomingXP Meta Attribute")
		);
	

	/*
	* Map of Damage Types to Resistances
	*/
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	/*
	* Map of Damage Types to Debuffs
	*/
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);

	/*
	 * Effects
	 */
	
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
FString("Tag granted when Hit Reacting")
	);

	GameplayTags.Effect_StaminaRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.StaminaRegen"),
FString("Tag granted for stamina regen")
	);

	/*
	 * Abilities
	*/
	GameplayTags.Abilities_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
FString("No Ability - like the nullptr for ability tags")
	);

	// Fire
	
	GameplayTags.Abilities_Fire_FireBolt= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
FString("FireBolt Ability Tag")
	);

	GameplayTags.Abilities_Fire_FireBlast= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
FString("FireBlast Ability Tag")
	);

	GameplayTags.Abilities_Fire_Flare= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Flare"),
FString("Flare Ability Tag")
	);
	
	GameplayTags.Abilities_Fire_Barrage= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Barrage"),
FString("Barrage Ability Tag")
	);

	GameplayTags.Abilities_Fire_Arrow= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Arrow"),
FString("Fire Arrow Ability Tag")
	);

	// Lightning
	
	GameplayTags.Abilities_Lightning_Electrocute= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
FString("Electrocute Ability Tag")
	);


	// Arcane 
	GameplayTags.Abilities_Arcane_ArcaneShards= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
FString("Arcane Shards Ability Tag")
	);

	GameplayTags.Abilities_Arcane_BattleCry= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.BattleCry"),
FString("BattleCry Ability Tag")
	);


	// Physical
	
	GameplayTags.Abilities_Physical_DefaultMelee= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.DefaultMelee"),
FString("DefaultMelee Ability Tag")
	);

	GameplayTags.Abilities_Physical_AbilityMelee= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.AbilityMelee"),
FString("AbilityMelee Ability Tag")
	);
	
	GameplayTags.Abilities_Physical_RadiantStrike= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.RadiantStrike"),
FString("RadiantStrike Ability Tag")
	);

	GameplayTags.Abilities_Physical_JumpAttack= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.JumpAttack"),
FString("JumpAttack Ability Tag")
	);

	GameplayTags.Abilities_Physical_SpinAttack= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.SpinAttack"),
FString("SpinAttack Ability Tag")
	);
	
	GameplayTags.Abilities_Physical_GroundSmash= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.GroundSmash"),
FString("GroundSmash Ability Tag")
	);

	GameplayTags.Abilities_Physical_GroundSmashCombo= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Physical.GroundSmashCombo"),
FString("GroundSmashCombo Ability Tag")
	);

	GameplayTags.Abilities_Physical_ThrowAxe= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Physical.ThrowAxe"),
FString("ThrowAxe Ability Tag")
	);
	
	GameplayTags.Abilities_Physical_ThrowAxe= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Physical.QuickAttack"),
FString("QuickAttack Ability Tag")
	);

	GameplayTags.Abilities_Physical_Arrow= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Physical.Arrow"),
FString("Arrow Ability Tag")
	);

	// Movement

	GameplayTags.Abilities_Movement_Jump= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Movement.Jump"),
FString("Jump Ability Tag")
	);

	GameplayTags.Abilities_Movement_Step= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Movement.Step"),
FString("Step Ability Tag")
	);

	GameplayTags.Abilities_Movement_Sprint= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Movement.Sprint"),
FString("Sprint Ability Tag")
	);


	GameplayTags.Abilities_Movement_Teleport= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Movement.Teleport"),
FString("Teleport Ability Tag")
	);

	GameplayTags.Abilities_Movement_Platform= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Movement.Platform"),
FString("Platform Ability Tag")
	);


	/*
	 * Defensive Abilities
	 */
	GameplayTags.Abilities_Defence_Block= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Defence.Block"),
FString("Block Ability Tag")
	);

	GameplayTags.Abilities_Defence_Heal= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Defence.Heal"),
FString("Heal Ability Tag")
	);

	GameplayTags.Abilities_Defence_Revive= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Defence.Revive"),
FString("Revive Ability Tag")
	);

	GameplayTags.Abilities_Defence_SelfRevive= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Defence.SelfRevive"),
FString("SelfRevive Ability Tag")
	);

	/* 
	 * Enemy Abilities
	*/
	GameplayTags.Abilities_Enemy_Melee= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Enemy.Default"),
FString("Default Enemy Attack Ability Tag")
	);

	GameplayTags.Abilities_Summon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
FString("Summon Ability Tag")
	);

	GameplayTags.Abilities_Enemy_Teleport= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Enemy.Teleport"),
FString("Enemy Teleport Ability Tag")
	);


	
	/*
	 * Passive Spells
	 */
	GameplayTags.Abilities_Passive_LifeSiphon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"),
		FString("Life Siphon"));

	GameplayTags.Abilities_Passive_ManaSiphon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"),
		FString("Mana Siphon")
	);

	
	GameplayTags.Abilities_Passive_HaloOfProtection= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"),
	FString("Halo Of Protection")
	);

	
	GameplayTags.Abilities_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
FString("HitReact Ability Tag")
	);
	
	GameplayTags.Abilities_Status_Eligible= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
FString("Eligible Status Tag")
	);

	GameplayTags.Abilities_Status_Equipped= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
FString("Equipped Status Tag")
	);
	
	GameplayTags.Abilities_Status_Locked= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
FString("Locked Status Tag")
	);
	
	GameplayTags.Abilities_Status_Unlocked= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
FString("Unlocked Status Tag")
	);

	GameplayTags.Abilities_Type_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
FString("No Type Tag")
	);
	
	GameplayTags.Abilities_Type_Offensive= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
FString("Offensive Type Tag")
	);
	
	GameplayTags.Abilities_Type_Passive= UGameplayTagsManager::Get().AddNativeGameplayTag(
FName("Abilities.Type.Passive"),
FString("Passive Type Tag")
	);
	
	
	
	
	
	
	
	
	/*
	* Cooldown
	*/
	
	GameplayTags.Cooldown_Fire_FireBolt= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
FString("FireBolt Cooldown Tag")
	);
	
	GameplayTags.Cooldown_Fire_Arrow= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.Arrow"),
FString("Fire Arrow Cooldown Tag")
	);
	
	GameplayTags.Cooldown_Physical_Arrow= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Physical.Arrow"),
FString("Arrow Cooldown Tag")
	);


	
	/*
	 * Combat Sockets
	*/
	
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
FString("Weapon")
	);
		
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
FString("Right Hand")
	);
		
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
FString("Left Hand")
	);

	/*
	 * Montage Disable Collision Notifier
	 */
	GameplayTags.Montage_Attack_DisableCollision = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.DisableCollision"),
FString("Disable Collision for weapons")
	);

	/*
	* Montage Tags
	*/
	GameplayTags.Montage_Attack_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.End"),
FString("Attack End")
	);
	
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
	FString("Attack 1")
	);
	
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
	FString("Attack 2")
	);
	
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
	FString("Attack 3")
	);
	
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
	FString("Attack 4")
	);
	
	GameplayTags.Montage_Attack_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.5"),
FString("Attack 5")
	);

	GameplayTags.Montage_Attack_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.6"),
			FString("Attack 6")
	);
	
	GameplayTags.Montage_Attack_RadiantStrike = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RadiantStrike"),
FString("RadiantStrike")
	);
	
	GameplayTags.Montage_Attack_RadiantStrikeEnd = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RadiantStrikeEnd"),
FString("RadiantStrikeEnd")
	);
	
	GameplayTags.Montage_Attack_Spin = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Spin"),
FString("Spin")
	);

	GameplayTags.Montage_Defence_Heal = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Montage.Defence.Heal"),
FString("Heal")
	);
	
	GameplayTags.Montage_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Jump"),
FString("Jump")
	);

	/*
	*  Player Tags
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
	FString("Block Input Pressed callback for input")
	);
	
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input")
	);
	
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input")
	);

	/* Gameplay Cues */

	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.FireBlast"),
	FString("FireBlast Gameplay Cue Tag")
);
}

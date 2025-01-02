#pragma once

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Melee UMETA(DisplayName = "Melee"),
	EAT_Ranged UMETA(DisplayName = "Ranged")
};


UENUM(BlueprintType, Blueprintable)
enum class ECombatStyle : uint8
{
	ECS_Unarmed UMETA(DisplayName = "Unarmed"),
	ECS_1HandWeapon UMETA(DisplayName = "One Handed Weapon"),
	ECS_2HandWeapon UMETA(DisplayName = "Two Handed Weapon"),
	ECS_Magic UMETA(DisplayName = "Magic"),
	ECS_Archery UMETA(DisplayName = "Archery"),
	ECS_UnarmedCombat UMETA(DisplayName = "Unarmed Combat"),
};

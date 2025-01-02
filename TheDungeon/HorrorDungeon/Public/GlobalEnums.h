#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Helmet UMETA(DisplayName = "Helmet"),
	EIT_Chest UMETA(DisplayName = "Chest"),
	EIT_Legs UMETA(DisplayName = "Legs"),
	EIT_Arms UMETA(DisplayName = "Arms"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Amulet UMETA(DisplayName = "Amulet"),
};

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	EIT_Idle UMETA(DisplayName = "Idle"),
	EIT_Walking UMETA(DisplayName = "Walking"),
	EIT_Jogging UMETA(DisplayName = "Jogging"),
	EIT_Sprinting UMETA(DisplayName = "Sprinting")

};

UENUM(BlueprintType, Blueprintable)
enum class EAIState : uint8
{
	EAIS_Passive UMETA(DisplayName = "Passive"),
	EAIS_Attacking UMETA(DisplayName = "Attacking"),
	EAIS_Frozen UMETA(DisplayName = "Frozen"),
	EAIS_Investigating UMETA(DisplayName = "Investigating"),
	EAIS_Dead UMETA(DisplayName = "Dead"),
	EAIS_Seeking UMETA(DisplayName = "Seeking"),
};

UENUM(BlueprintType, Blueprintable)
enum class EAISense : uint8
{
	EAISense_None UMETA(DisplayName = "None"),
	EAISense_Sight UMETA(DisplayName = "Sight"),
	EAISense_Hearing UMETA(DisplayName = "Hearing"),
	EAISense_Damage UMETA(DisplayName = "Damage")
};

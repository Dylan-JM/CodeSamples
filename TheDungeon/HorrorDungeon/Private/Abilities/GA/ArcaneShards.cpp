// Copyright DM


#include "Abilities/GA/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Mana Cost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Summon shards of arcane energy, "
			"causing radial damage of </>"
			// Damage
			"<Damage>%d</><Default> at the shard origin.</>"

			//Values
			), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			// Additional Number of Bolts
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
			// Damage
			"<Damage>%d</><Default> at the shard origins.</>"

			//Values
			), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
	// Titles
	"<Title>NEXT LEVEL: </>\n\n"

	// Level
	"<Small>Level: </><Level>%d</>\n"
	// ManaCost
	"<Small>ManaCost: </><ManaCost>%.1f</>\n"
	// Cooldown
	"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
	// Additional Number of Bolts
	"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
	// Damage
	"<Damage>%d</><Default> at the shard origins.</>"

	//Values
	), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);
}

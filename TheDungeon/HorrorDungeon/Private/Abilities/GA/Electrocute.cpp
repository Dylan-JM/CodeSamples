// Copyright DM


#include "Abilities/GA/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>ELECTROCUTE</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Mana Cost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </>"
			// Damage
			"<Damage>%d</><Default> "
			"lightning damage with a chance to stun</>"

			//Values
			), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>ELECTROCUTE</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			// Additional Number of Bolts
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, causing </>"
			// Damage
			"<Damage>%d</><Default> "
			"lightning damage with a chance to burn</>"

			//Values
			), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
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
	"<Default>Emits a beam of lightning, "
	"propagating to %d additional targets nearby, causing </>"
	// Damage
	"<Damage>%d</><Default> "
	"lightning damage with a chance to burn</>"

	//Values
	), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
}

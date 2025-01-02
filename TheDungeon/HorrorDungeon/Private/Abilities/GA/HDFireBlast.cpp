// Copyright DM


#include "Abilities/GA/HDFireBlast.h"

#include "Abilities/HDAbilitySystemLibrary.h"
#include "Actor/HDFireBall.h"

FString UHDFireBlast::GetDescription(int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	

	return FString::Printf(TEXT(
	// Titles
	"<Title>FIRE BLAST</>\n\n"

	// Level
	"<Small>Level: </><Level>%d</>\n"
	// Mana Cost
	"<Small>ManaCost: </><ManaCost>%.1f</>\n"
	// Cooldown
	"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	// Number of fireballs
	"<Default>Launches %d</>"
	"<Default>fire balls in all directions, each coming back and </>"
	"<Default>exploding upon return, causing </>"
			
	// Damage
	"<Damage>%d</><Default> "
	"radial fire damage with a chance to burn</>"

	//Values
	), Level, ManaCost, Cooldown, NumFireballs, ScaledDamage);

}

FString UHDFireBlast::GetNextLevelDescription(int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	

	return FString::Printf(TEXT(
	// Titles
	"<Title>NEXT LEVEL:</>\n\n"

	// Level
	"<Small>Level: </><Level>%d</>\n"
	// Mana Cost
	"<Small>ManaCost: </><ManaCost>%.1f</>\n"
	// Cooldown
	"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	// Number of fireballs
	"<Default>Launches %d</>"
	"<Default>fire balls in all directions, each coming back and </>"
	"<Default>exploding upon return, causing </>"
			
	// Damage
	"<Damage>%d</><Default> "
	"radial fire damage with a chance to burn</>"

	//Values
	), Level, ManaCost, Cooldown, NumFireballs, ScaledDamage);
}

TArray<AHDFireBall*> UHDFireBlast::SpawnFireBalls()
{
	TArray<AHDFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UHDAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireballs);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AHDFireBall* FireBall = GetWorld()->SpawnActorDeferred<AHDFireBall>(FireballClass, SpawnTransform, GetOwningActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}

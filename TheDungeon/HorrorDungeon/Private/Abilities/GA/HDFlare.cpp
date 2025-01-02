// Copyright DM


#include "Abilities/GA/HDFlare.h"

#include "Abilities/HDAbilitySystemLibrary.h"
#include "Actor/HDFlareProjectile.h"


FString UHDFlare::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UHDFlare::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

TArray<AHDFlareProjectile*> UHDFlare::SpawnFlare()
{
	TArray<AHDFlareProjectile*> Flares;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UHDAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFlares);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AHDFlareProjectile* Flare = GetWorld()->SpawnActorDeferred<AHDFlareProjectile>(FlareClass, SpawnTransform, GetOwningActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Flare->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Flare->SetOwner(GetAvatarActorFromActorInfo());

		Flare->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		Flare->SetOwner(GetAvatarActorFromActorInfo());
		
		Flares.Add(Flare);

		Flare->FinishSpawning(SpawnTransform);
	}

	return Flares;
}


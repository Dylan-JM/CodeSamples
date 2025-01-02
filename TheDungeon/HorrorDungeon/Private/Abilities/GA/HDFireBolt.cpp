// Copyright DM


#include "Abilities/GA/HDFireBolt.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Actor/HDProjectile.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "Weapons/BaseWeapon.h"

FString UHDFireBolt::GetDescription(int32 Level)
{
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Mana Cost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing </>"
			// Damage
			"<Damage>%d</><Default> "
			"fire damage with a chance to burn</>"

			//Values
			), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Titles
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			// Number of Bolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing </>"
			// Damage
			"<Damage>%d</><Default> "
			"fire damage with a chance to burn</>"

			//Values
			), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
	}
}

FString UHDFireBolt::GetNextLevelDescription(int32 Level)
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

		// Number of Bolts
		"<Default>Launches %d bolts of fire, "
		"exploding on impact and dealing </>"
		// Damage
		"<Damage>%d</><Default> "
		"fire damage with a chance to burn</>"

		//Values
		), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
}

void UHDFireBolt::SpawnProjectiles(bool bOverridePitch, float PitchOverride)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		FRotator TargetRotation;
		FTransform OutTransform;
		const UStaticMeshSocket* TipSocket = PlayerCharacter->EquippedWeapon1->GetItemMesh()->GetSocketByName(FName("TipSocket"));
		TipSocket->GetSocketTransform(OutTransform, PlayerCharacter->EquippedWeapon1->GetItemMesh());
		
		TargetRotation = PlayerCharacter->GetActorRotation();
		
		if (bOverridePitch) TargetRotation.Pitch = PitchOverride;
		const FVector Forward = TargetRotation.Vector();
		const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
		TArray<FRotator> Rotations = UHDAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);


		HitTargetLoc = PlayerCharacter->GetTraceResult(TraceDistance, true).ImpactPoint;
		for (const FRotator& Rot : Rotations)
		{
			ServerSpawnProjectiles(OutTransform, Rot, HitTargetLoc);
		}
	}		
}

void UHDFireBolt::MulticastSpawnProjectiles_Implementation(FTransform OutTransform, FRotator TargetRotation, FVector TempHitTargetLoc)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (PlayerCharacter && PlayerCharacter->IsLocallyControlled() && !PlayerCharacter->HasAuthority()) return;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetAvatarActorFromActorInfo();
		TObjectPtr<AHDProjectile> SpawnedProjectile = nullptr;
		SpawnedProjectile = GetWorld()->SpawnActor<AHDProjectile>(ProjectileClass, OutTransform.GetLocation(), TargetRotation ,SpawnParams);
		SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		SpawnedProjectile->SetOwner(PlayerCharacter);

		
		SpawnedProjectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		SpawnedProjectile->HomingTargetSceneComponent->SetWorldLocation(TempHitTargetLoc);
		SpawnedProjectile->ProjectileMovement->HomingTargetComponent = SpawnedProjectile->HomingTargetSceneComponent;
		
		SpawnedProjectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		SpawnedProjectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
	}
}

void UHDFireBolt::ServerSpawnProjectiles_Implementation(FTransform OutTransform, FRotator TargetRotation, FVector TempHitTargetLoc)
{
	//MulticastSpawnProjectiles(OutTransform, TargetRotation, TempHitTargetLoc);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (PlayerCharacter && PlayerCharacter->IsLocallyControlled() && !PlayerCharacter->HasAuthority()) return;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetAvatarActorFromActorInfo();
		TObjectPtr<AHDProjectile> SpawnedProjectile = nullptr;
		SpawnedProjectile = GetWorld()->SpawnActor<AHDProjectile>(ProjectileClass, OutTransform.GetLocation(), TargetRotation ,SpawnParams);
		SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		SpawnedProjectile->SetOwner(PlayerCharacter);

		
		SpawnedProjectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		SpawnedProjectile->HomingTargetSceneComponent->SetWorldLocation(TempHitTargetLoc);
		SpawnedProjectile->ProjectileMovement->HomingTargetComponent = SpawnedProjectile->HomingTargetSceneComponent;
		
		SpawnedProjectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		SpawnedProjectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
	}
}

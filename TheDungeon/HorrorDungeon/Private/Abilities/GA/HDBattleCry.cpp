// Copyright DM


#include "Abilities/GA/HDBattleCry.h"
#include "Actor/HDProjectile.h"
#include "Enemy/BaseEnemy.h"

void UHDBattleCry::SpawnExplosions(FVector SpawnLocation)
{
	TObjectPtr<AHDProjectile> SpawnedProjectile = nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	FRotator TargetRotation;
	
	SpawnedProjectile = GetWorld()->SpawnActor<AHDProjectile>(ProjectileClass, SpawnLocation, TargetRotation, SpawnParams);
	
	SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

}

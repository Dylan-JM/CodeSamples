// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDProjectileSpell.h"
#include "HDFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDFireBolt : public UHDProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectiles(bool bOverridePitch, float PitchOverride);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnProjectiles(FTransform OutTransform, FRotator TargetRotation, FVector TempHitTargetLoc);
	UFUNCTION(Server, Reliable)
	void ServerSpawnProjectiles(FTransform OutTransform, FRotator TargetRotation, FVector TempHitTargetLoc);

protected:
	UPROPERTY(EditDefaultsOnly, Category= "FireBolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category= "FireBolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category= "FireBolt")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category= "FireBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category= "FireBolt")
	bool bLaunchHomingProjectiles = true;
	
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "HDDamageGameplayAbility.h"
#include "HDProjectileSpell.generated.h"


class AHDProjectile;
class UGameplayEffect;

UCLASS()
class HORRORDUNGEON_API UHDProjectileSpell : public UHDDamageGameplayAbility
{
	GENERATED_BODY()



protected:

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile();

	UFUNCTION(Server, Reliable)
	void ServerSpawnProjectile(FTransform OutTransform, FRotator TargetRotation);

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHDProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	UPROPERTY(BlueprintReadOnly)
	FVector HitTargetLoc;
};

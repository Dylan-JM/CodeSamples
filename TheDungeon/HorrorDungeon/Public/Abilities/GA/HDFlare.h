// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDDamageGameplayAbility.h"
#include "HDFlare.generated.h"

class AHDFlareProjectile;
/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDFlare : public UHDDamageGameplayAbility
{
	GENERATED_BODY()


public:
	virtual FString GetDescription(int32 Level) override;;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AHDFlareProjectile*> SpawnFlare();

protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	int32 NumFlares = 1;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHDFlareProjectile> FlareClass;
	
};

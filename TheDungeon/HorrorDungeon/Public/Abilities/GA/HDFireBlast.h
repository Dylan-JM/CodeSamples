// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDDamageGameplayAbility.h"
#include "HDFireBlast.generated.h"

class AHDFireBall;
/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDFireBlast : public UHDDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AHDFireBall*> SpawnFireBalls();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	int32 NumFireballs = 12;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHDFireBall> FireballClass;
};

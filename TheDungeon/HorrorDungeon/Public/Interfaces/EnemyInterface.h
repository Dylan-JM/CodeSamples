// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GlobalEnums.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

class APatrolRoute;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORDUNGEON_API IEnemyInterface
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float SetMovementSpeed(EMovementSpeed MovementSpeed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	APatrolRoute* GetPatrolRoute() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetIdealRange(float& AttackRadius, float& DefendRadius);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JumpToDestination(FVector Destination);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AttackStart(AActor* AttackTarget, int32 TokensNeeded);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackEnd(AActor* AttackTarget);
};

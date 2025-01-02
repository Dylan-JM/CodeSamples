// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Attack_Mage.generated.h"

class ABaseEnemy;

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_Attack_Mage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Attack_Mage();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AttackTag;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer SummonTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinionSpawnThreshold = 3.f;
	
	UPROPERTY(EditAnywhere)
	int32 TokensNeeded;

private:

	UFUNCTION()
	void AttackWhenTeleportFinished();
	UFUNCTION()
	void AttackFinished();

	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;
};

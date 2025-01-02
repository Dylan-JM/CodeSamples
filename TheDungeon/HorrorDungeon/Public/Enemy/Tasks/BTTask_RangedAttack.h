// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangedAttack.generated.h"

class ABaseEnemy;
class AEnemyAIController;


/**
 * 
 */
UCLASS

()
class HORRORDUNGEON_API UBTTask_RangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_RangedAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	int32 TokensNeeded;
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AttackTag;
private:

	UFUNCTION()
	void AttackFinished();

	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> Controller;
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MeleeAttack.generated.h"

class AEnemyAIController;
class ABaseEnemy;
struct FPathFollowingResult;
struct FAIRequestID;

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_MeleeAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackRadiusKey;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AttackTag;
	
	UPROPERTY(EditAnywhere)
	int32 TokensNeeded;
	
	FDelegateHandle MoveToFinishedDelegate;
private:
	
	UFUNCTION()
	void AttackFinished();
	
	void Attack(FAIRequestID RequestID, const FPathFollowingResult&);

	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> Controller;
};

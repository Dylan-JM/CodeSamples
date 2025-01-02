// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Boss_Attack.generated.h"

class ABaseEnemy;

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_Boss_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTT_Boss_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AttackTag;
	
private:

	UFUNCTION()
	void AttackFinished();

	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;
};

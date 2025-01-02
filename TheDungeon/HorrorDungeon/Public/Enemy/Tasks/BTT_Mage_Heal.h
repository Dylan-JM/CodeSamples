// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Mage_Heal.generated.h"

class ABaseEnemy;

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_Mage_Heal : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Mage_Heal();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	
	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;

	UFUNCTION()
	void HealFinished();

};

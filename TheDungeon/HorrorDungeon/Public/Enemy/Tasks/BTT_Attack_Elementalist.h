// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Attack_Elementalist.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_Attack_Elementalist : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Attack_Elementalist();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector CombatTargetKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinionSpawnThreshold;
	
};

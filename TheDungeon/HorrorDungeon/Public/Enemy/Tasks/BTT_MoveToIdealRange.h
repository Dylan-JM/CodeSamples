// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveToIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_MoveToIdealRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTT_MoveToIdealRange();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector IdealRangeKey;
	
};

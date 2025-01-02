// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_ClearFocus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTT_ClearFocus();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

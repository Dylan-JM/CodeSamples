// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetStateAsPassive.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_SetStateAsPassive : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTT_SetStateAsPassive();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

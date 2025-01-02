// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetInCombat.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_SetInCombat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

	UBTT_SetInCombat();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	bool InCombat;
};

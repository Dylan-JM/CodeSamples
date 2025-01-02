// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTS_UpdateDistanceToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

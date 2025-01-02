// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveAlongPatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTTask_MoveAlongPatrolPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AcceptanceRadius = 5.f;
	
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GlobalEnums.h"
#include "BTT_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_SetMovementSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
    
	UBTT_SetMovementSpeed();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
	UPROPERTY(EditAnywhere)
	EMovementSpeed MovementSpeed;
};

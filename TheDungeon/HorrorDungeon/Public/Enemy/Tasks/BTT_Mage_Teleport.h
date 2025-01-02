// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Mage_Teleport.generated.h"

class ABaseEnemy;

/**
 * 
 */
UCLASS

()
class HORRORDUNGEON_API UBTT_Mage_Teleport : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTT_Mage_Teleport();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer TeleportTag;
private:

	UFUNCTION()
	void TeleportFinished();
	
	UPROPERTY()
	TObjectPtr<ABaseEnemy> Enemy;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;
};

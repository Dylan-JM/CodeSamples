// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ApplyEffectToSelf.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTT_ApplyEffectToSelf : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTT_ApplyEffectToSelf();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectToApply;
};

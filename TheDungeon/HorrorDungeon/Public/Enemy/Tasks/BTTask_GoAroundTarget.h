// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GoAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UBTTask_GoAroundTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_GoAroundTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 300.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector MoveToLocationSelector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetSelector;

	
};

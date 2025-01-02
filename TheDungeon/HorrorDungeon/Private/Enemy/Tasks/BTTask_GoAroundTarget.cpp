// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Tasks/BTTask_GoAroundTarget.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTTask_GoAroundTarget::UBTTask_GoAroundTarget()
{
	NodeName = TEXT("Go Around Target");
}

EBTNodeResult::Type UBTTask_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow"));
		if (TargetActor)
		{
			FVector NewLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(GetWorld(), TargetActor->GetActorLocation(), NewLocation, Radius);
			BlackboardComp->SetValueAsVector("MoveToLocation", NewLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

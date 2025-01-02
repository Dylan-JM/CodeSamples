// Copyright DM


#include "Enemy/Tasks/BTT_MoveToIdealRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_MoveToIdealRange::UBTT_MoveToIdealRange()
{
	NodeName = TEXT("Move To Ideal Range");
}

EBTNodeResult::Type UBTT_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		float IdealRange = BlackboardComp->GetValueAsFloat(IdealRangeKey.SelectedKeyName);
		Controller->MoveToLocation(TargetActor->GetActorLocation(), IdealRange);
	}
	return EBTNodeResult::Succeeded;
	
}

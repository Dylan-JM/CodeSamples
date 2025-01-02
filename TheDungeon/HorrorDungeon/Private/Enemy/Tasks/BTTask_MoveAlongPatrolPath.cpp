// Copyright DM


#include "Enemy/Tasks/BTTask_MoveAlongPatrolPath.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/PatrolRoute.h"
#include "Enemy/Controller/EnemyAIController.h"

EBTNodeResult::Type UBTTask_MoveAlongPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	
	if (APatrolRoute* PatrolRoute = IEnemyInterface::Execute_GetPatrolRoute(Enemy))
	{
		Controller->MoveToLocation(PatrolRoute->GetSplinePointAsWorldPosition(), AcceptanceRadius);
		PatrolRoute->IncrementPatrolRoute();
	}
	return EBTNodeResult::Succeeded;
}

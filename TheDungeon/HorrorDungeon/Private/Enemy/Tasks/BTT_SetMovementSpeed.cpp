// Copyright DM


#include "Enemy/Tasks/BTT_SetMovementSpeed.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_SetMovementSpeed::UBTT_SetMovementSpeed()
{
	NodeName = TEXT("Set Movement Speed");
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (!IsValid(Controller) || !IsValid(BaseEnemy)) return EBTNodeResult::Failed;

	BaseEnemy->SetMovementSpeed_Implementation(MovementSpeed);
	return EBTNodeResult::Succeeded;
}

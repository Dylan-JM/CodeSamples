// Copyright DM


#include "Enemy/Tasks/BTT_SetInCombat.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_SetInCombat::UBTT_SetInCombat()
{
	NodeName = TEXT("Set In Combat");
}

EBTNodeResult::Type UBTT_SetInCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (!IsValid(Controller) || !IsValid(BaseEnemy)) return EBTNodeResult::Failed;
	
	BaseEnemy->SetInCombat(InCombat);

	return EBTNodeResult::Succeeded;

}

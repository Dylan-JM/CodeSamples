// Copyright DM


#include "Enemy/Tasks/BTT_SetStateAsPassive.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_SetStateAsPassive::UBTT_SetStateAsPassive()
{
	NodeName = TEXT("Set State As Passive");
}

EBTNodeResult::Type UBTT_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller)) return EBTNodeResult::Failed;

	Controller->SetAIState(0);
	return EBTNodeResult::Succeeded;
}

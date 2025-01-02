// Copyright DM


#include "Enemy/Tasks/BTT_ClearFocus.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_ClearFocus::UBTT_ClearFocus()
{
	NodeName = TEXT("Clear Focus");
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());

	if (BaseEnemy)
	{
		BaseEnemy->bUseControllerRotationYaw = false;
	}
	
	if (Controller)
	{
		Controller->ClearFocus(0);
	}
	return EBTNodeResult::Succeeded;
}

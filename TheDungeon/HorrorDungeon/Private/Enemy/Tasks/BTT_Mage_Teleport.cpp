// Copyright DM


#include "Enemy/Tasks/BTT_Mage_Teleport.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_Mage_Teleport::UBTT_Mage_Teleport()
{
	NodeName = TEXT("Mage Teleport");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Mage_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Controller == nullptr || Enemy == nullptr) return EBTNodeResult::Failed;
	Enemy->TeleportFinishedDelegate.AddDynamic(this, &UBTT_Mage_Teleport::TeleportFinished);
	if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TeleportTag)) return EBTNodeResult::Failed;
	
	return EBTNodeResult::InProgress;
}

void UBTT_Mage_Teleport::TeleportFinished()
{
	Enemy->TeleportFinishedDelegate.Clear();
	if (MyOwnerComp)
	{
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
		if (OwnerComp)
		{
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

// Copyright DM


#include "Enemy/Tasks/BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Enemy)
	{
		Enemy->AttackFinishedDelegate.AddDynamic(this, &UBTT_Attack::AttackFinished);
		if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			Enemy->Execute_SetCombatTarget(Enemy, Cast<AActor>(BlackboardComp->GetValueAsObject(CombatTargetKey.SelectedKeyName)));
		}
		Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Attack::AttackFinished()
{
	Enemy->AttackFinishedDelegate.Clear();
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

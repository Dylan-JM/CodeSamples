// Copyright DM


#include "Enemy/Tasks/BTT_Boss_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_Boss_Attack::UBTT_Boss_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Boss_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Enemy)
	{
		Enemy->AttackFinishedDelegate.AddDynamic(this, &UBTT_Boss_Attack::AttackFinished);
		if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			Enemy->Execute_SetCombatTarget(Enemy, Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow")));
		}
		Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Boss_Attack::AttackFinished()
{
	Enemy->AttackFinishedDelegate.Clear();
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

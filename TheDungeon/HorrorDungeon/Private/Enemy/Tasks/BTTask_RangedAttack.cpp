// Copyright DM


#include "Enemy/Tasks/BTTask_RangedAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Controller == nullptr || Enemy == nullptr) return EBTNodeResult::Failed;
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		if (Enemy->Execute_AttackStart(Enemy, AttackTarget, TokensNeeded))
		{
			// Set Focus
			Controller->SetFocus(AttackTarget);
			Enemy->bUseControllerRotationYaw = true;

			// Set In Combat
			Enemy->SetInCombat(true);

			// Attack
			Enemy->AttackFinishedDelegate.AddDynamic(this, &UBTTask_RangedAttack::AttackFinished);
			Enemy->SetCombatTarget_Implementation(AttackTarget);

			if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag))
			{
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_RangedAttack::AttackFinished()
{
	Enemy->AttackFinishedDelegate.Clear();
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

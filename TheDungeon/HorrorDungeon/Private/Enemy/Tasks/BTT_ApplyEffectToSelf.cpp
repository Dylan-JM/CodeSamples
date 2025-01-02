// Copyright DM


#include "Enemy/Tasks/BTT_ApplyEffectToSelf.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_ApplyEffectToSelf::UBTT_ApplyEffectToSelf()
{
	NodeName = TEXT("Apply Effect To Self");
}

EBTNodeResult::Type UBTT_ApplyEffectToSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (!IsValid(Controller) || !IsValid(BaseEnemy)) return EBTNodeResult::Failed;

	FGameplayEffectContextHandle GameplayEffectContext;
	BaseEnemy->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(EffectToApply.GetDefaultObject(), 1.f, GameplayEffectContext);

	return EBTNodeResult::Succeeded;
}

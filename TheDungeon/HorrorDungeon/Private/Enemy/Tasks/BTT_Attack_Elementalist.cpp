// Copyright DM


#include "Enemy/Tasks/BTT_Attack_Elementalist.h"

#include "BlueprintGameplayTagLibrary.h"
#include "HDGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_Attack_Elementalist::UBTT_Attack_Elementalist()
{
	NodeName = TEXT("Elementalist Attack");
}

EBTNodeResult::Type UBTT_Attack_Elementalist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (BaseEnemy)
	{
		if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			BaseEnemy->Execute_SetCombatTarget(BaseEnemy, Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow")));
		}
		FGameplayTagContainer AttackTagContainer;
		if (BaseEnemy->Execute_GetMinionCount(BaseEnemy) < MinionSpawnThreshold)
		{
			AttackTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHDGameplayTags::Get().Abilities_Summon);
		}
		else
		{
			AttackTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHDGameplayTags::Get().Abilities_Enemy_Melee);
		}
		BaseEnemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTagContainer);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
}

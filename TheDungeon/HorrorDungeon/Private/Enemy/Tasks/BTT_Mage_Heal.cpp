// Copyright DM


#include "Enemy/Tasks/BTT_Mage_Heal.h"
#include "BlueprintGameplayTagLibrary.h"
#include "HDGameplayTags.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"



UBTT_Mage_Heal::UBTT_Mage_Heal()
{
	NodeName = TEXT("Mage Heal");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Mage_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Enemy)
	{
		Enemy->HealFinishedDelegate.AddDynamic(this, &UBTT_Mage_Heal::HealFinished);
		FGameplayTagContainer HealTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHDGameplayTags::Get().Abilities_Defence_Heal);
		if (!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(HealTagContainer))
		{
			Enemy->HealFinishedDelegate.Clear();
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Mage_Heal::HealFinished()
{
	Enemy->HealFinishedDelegate.Clear();
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

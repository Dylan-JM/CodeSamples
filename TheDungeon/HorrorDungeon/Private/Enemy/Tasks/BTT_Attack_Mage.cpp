// Copyright DM


#include "Enemy/Tasks/BTT_Attack_Mage.h"
#include "BlueprintGameplayTagLibrary.h"
#include "HDGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_Attack_Mage::UBTT_Attack_Mage()
{
	NodeName = TEXT("Mage Attack");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Attack_Mage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Controller == nullptr || Enemy == nullptr) return EBTNodeResult::Failed;
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		if (Enemy->Execute_AttackStart(Enemy, AttackTarget, TokensNeeded))
		{
			Controller->SetFocus(AttackTarget);
			Enemy->bUseControllerRotationYaw = true;
			Enemy->SetInCombat(true);

			FGameplayTagContainer TeleportTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHDGameplayTags::Get().Abilities_Movement_Teleport);
			Enemy->TeleportFinishedDelegate.AddDynamic(this, &UBTT_Attack_Mage::AttackWhenTeleportFinished);
			if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TeleportTagContainer))
			{
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Attack_Mage::AttackWhenTeleportFinished()
{
	Enemy->TeleportFinishedDelegate.Clear();
	Enemy->AttackFinishedDelegate.AddDynamic(this, &UBTT_Attack_Mage::AttackFinished);
	
	Enemy->Execute_SetCombatTarget(Enemy, AttackTarget);
	if (SummonTag.IsValid() && Enemy->Execute_GetMinionCount(Enemy) < MinionSpawnThreshold)
	{
		if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(SummonTag))
		{
			Enemy->Execute_AttackEnd(this, AttackTarget);
			UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
			if (OwnerComp)
			{
				FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
	else
	{
		if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag))
		{
			Enemy->Execute_AttackEnd(this, AttackTarget);
			UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
			if (OwnerComp)
			{
				FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

void UBTT_Attack_Mage::AttackFinished()
{
	Enemy->AttackFinishedDelegate.Clear();
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Yellow,GetOuter()->GetName());
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

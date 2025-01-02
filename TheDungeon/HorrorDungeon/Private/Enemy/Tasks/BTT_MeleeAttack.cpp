// Copyright DM


#include "Enemy/Tasks/BTT_MeleeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h" 
#include "AITypes.h" 

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (Controller == nullptr || Enemy == nullptr) return EBTNodeResult::Failed;
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
		if (Enemy->Execute_AttackStart(Enemy, AttackTarget, TokensNeeded))
		{
			// Clear Focus
			Enemy->SetMovementSpeed_Implementation(EMovementSpeed::EIT_Sprinting);
			Enemy->bUseControllerRotationYaw = false;
			Controller->ClearFocus(0);

			// Move To Ideal Range
			float IdealRange = BlackboardComp->GetValueAsFloat(AttackRadiusKey.SelectedKeyName);
			Controller->MoveToActor(AttackTarget, IdealRange);

			MoveToFinishedDelegate = Controller->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_MeleeAttack::Attack);
			
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::AttackFinished()
{
	Enemy->AttackFinishedDelegate.Clear();
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_MeleeAttack::Attack(FAIRequestID RequestID, const FPathFollowingResult&)
{
	Controller->GetPathFollowingComponent()->OnRequestFinished.Remove(MoveToFinishedDelegate);
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	
	// Set Focus
	Controller->SetFocus(AttackTarget);
	Enemy->bUseControllerRotationYaw = true;

	// Attack
	Enemy->AttackFinishedDelegate.AddDynamic(this, &UBTT_MeleeAttack::AttackFinished);
	Enemy->SetCombatTarget_Implementation(AttackTarget);
			
	if(!Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AttackTag))
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

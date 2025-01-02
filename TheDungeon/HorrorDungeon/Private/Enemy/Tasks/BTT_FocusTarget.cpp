// Copyright DM


#include "Enemy/Tasks/BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

UBTT_FocusTarget::UBTT_FocusTarget()
{
	NodeName = TEXT("Focus Target");
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (!IsValid(Controller)) return EBTNodeResult::Failed;
	

	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FocusTarget.SelectedKeyName));
		if (TargetActor)
		{
			BaseEnemy->bUseControllerRotationYaw = true;
			Controller->SetFocus(TargetActor);
		}
		else
		{
			FVector TargetLocation = BlackboardComp->GetValueAsVector(FocusTarget.SelectedKeyName);
			if (UAIBlueprintHelperLibrary::IsValidAILocation(TargetLocation))
			{
				Controller->SetFocalPoint(TargetLocation);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

// Copyright DM


#include "Enemy/Services/BTS_StopAttackingIfTargetDead.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Interfaces/CombatInterface.h"

void UBTS_StopAttackingIfTargetDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		if (AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow")))
		{
			if (Target->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Target))
			{
				if (AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
				{
					Controller->SetAIState(0);
				}
			}
		}
	}
}

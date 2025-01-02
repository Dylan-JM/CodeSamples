// Copyright DM


#include "Enemy/Services/BTS_UpdateDistanceToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

void UBTS_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* OwningPawn = Controller->GetPawn();
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow"));
		if (TargetActor)
		{
			BlackboardComp->SetValueAsFloat("DistanceToTarget",UKismetMathLibrary::Vector_Distance(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation()));
		}
	}
}

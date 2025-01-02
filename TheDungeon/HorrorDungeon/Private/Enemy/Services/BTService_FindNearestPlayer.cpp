#include "Enemy/Services/BTService_FindNearestPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/PlayerCharacter.h"


void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* OwningPawn = Controller->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
    
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
            
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ClosestActor))
				{
					if (ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OwningPawn))
					{
						if (BaseEnemy->AttackType == EAttackType::EAT_Melee)
						{
							if (PlayerCharacter->MaxNumOfMeleeAttackers != PlayerCharacter->MeleeAttackers.Num())
							{
								PlayerCharacter->MeleeAttackers.AddUnique(OwningPawn);
							}
						}
						else if (BaseEnemy->AttackType == EAttackType::EAT_Ranged)
						{
							if (PlayerCharacter->MaxNumOfRangedAttackers != PlayerCharacter->RangedAttackers.Num())
							{
								PlayerCharacter->RangedAttackers.AddUnique(OwningPawn);
							}
						}
					}

				}
			}
		}
	}
	
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsFloat("DistanceToTarget", ClosestDistance);
		BlackboardComp->SetValueAsObject("TargetToFollow", ClosestActor);
	}
	
}

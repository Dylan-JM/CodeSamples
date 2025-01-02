// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Tasks/MyBTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

UMyBTTask_FindPlayerLocation::UMyBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UMyBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character
	if (UBlackboardComponent* bc = OwnerComp.GetBlackboardComponent())
	{
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(bc->GetValueAsObject("TargetPlayer"));
		if (TargetPlayer)
		{
			// get player location to use as an origin
			FVector PlayerLocation = TargetPlayer->GetActorLocation();
			if (SearchRandom)
			{
				FNavLocation Loc;

				if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

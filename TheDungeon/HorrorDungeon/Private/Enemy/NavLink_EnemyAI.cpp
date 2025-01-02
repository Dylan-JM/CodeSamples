// Copyright DM


#include "Enemy/NavLink_EnemyAI.h"

#include "Interfaces/EnemyInterface.h"

ANavLink_EnemyAI::ANavLink_EnemyAI()
{
	bSmartLinkIsRelevant = true;
	OnSmartLinkReached.AddDynamic(this, &ANavLink_EnemyAI::SmartLinkReached);
}

void ANavLink_EnemyAI::SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Yellow, FString(TEXT("LinkReached")));
	IEnemyInterface::Execute_JumpToDestination(MovingActor, DestinationPoint);
}


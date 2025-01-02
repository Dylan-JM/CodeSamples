// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLink_EnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API ANavLink_EnemyAI : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	ANavLink_EnemyAI();

protected:

	UFUNCTION()
	void SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);
};

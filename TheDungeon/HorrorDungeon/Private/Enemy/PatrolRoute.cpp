// Copyright DM


#include "Enemy/PatrolRoute.h"

#include "Components/SplineComponent.h"


APatrolRoute::APatrolRoute()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolRoute = CreateDefaultSubobject<USplineComponent>("PatrolRoute");

}

void APatrolRoute::IncrementPatrolRoute()
{
	PatrolIndex = PatrolIndex + Direction;

	if (PatrolIndex == (PatrolRoute->GetNumberOfSplinePoints() -1))
	{
		Direction = -1;
	}
	else if (PatrolIndex == 0)
	{
		Direction = 1;
	}

}

FVector APatrolRoute::GetSplinePointAsWorldPosition() const
{
	return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}


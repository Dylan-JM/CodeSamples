// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class HORRORDUNGEON_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolRoute();

	UFUNCTION(BlueprintCallable)
	void IncrementPatrolRoute();

	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointAsWorldPosition() const;

	UPROPERTY(meta=(ClampMin = -1), meta=(ClampMax = 1))
	int32 Direction;
	
	int32 PatrolIndex;

protected:

private:

	TObjectPtr<USplineComponent> PatrolRoute;


};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Actor/EquippableActor.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API AShield : public AEquippableActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	float DamageConsumptionMultiplier = 1.f;
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UElectrocute : public UHDBeamSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;;
	virtual FString GetNextLevelDescription(int32 Level) override;
};

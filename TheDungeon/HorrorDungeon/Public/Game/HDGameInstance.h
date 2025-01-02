// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Abilities/Data/CharacterClassInfo.h"
#include "HDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;
};

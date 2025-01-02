// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerController/BasePlayerController.h"
#include "UserSettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UUserSettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FUserSettings UserSettings;
	
};

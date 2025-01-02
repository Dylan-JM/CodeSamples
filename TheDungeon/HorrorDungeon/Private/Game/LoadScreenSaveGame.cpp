// Copyright DM


#include "Game/LoadScreenSaveGame.h"

FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}

void ULoadScreenSaveGame::ClearEquippedItems()
{
	EquippedWeapon1 = nullptr;
	EquippedOffHandItem = nullptr;
	EquippedHelmet = nullptr;
	EquippedChestplate = nullptr;
	EquippedGaunlets = nullptr;
	EquippedLegs = nullptr;
	EquippedAmulet1 = nullptr;
	EquippedAmulet2 = nullptr;
	EquippedAmulet3 = nullptr;
}

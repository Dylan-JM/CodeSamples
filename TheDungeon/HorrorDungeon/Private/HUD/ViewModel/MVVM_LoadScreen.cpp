// Copyright DM


#include "HUD/ViewModel/MVVM_LoadScreen.h"

#include "HorrorDungeonGameMode.h"
#include "Game/HDGameInstance.h"
#include "HUD/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName, ECharacterClass InCharacterClass)
{
	AHorrorDungeonGameMode* HDGameMode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(HDGameMode))
	{
		GEngine->AddOnScreenDebugMessage(1,15,FColor::Magenta, FString("Please Switch to Single Player"));
		return;
	}

	LoadSlots[Slot]->SetMapName(HDGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->SetCharacterClass(InCharacterClass);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = HDGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->MapAssetName = HDGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	
	HDGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(HDGameMode->GetGameInstance());
	HDGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	HDGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	HDGameInstance->PlayerStartTag = HDGameMode->DefaultPlayerStartTag;
	HDGameInstance->CharacterClass = LoadSlots[Slot]->GetCharacterClass();
	
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AHorrorDungeonGameMode::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
	
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AHorrorDungeonGameMode* HDGameMode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(this));
	UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(HDGameMode->GetGameInstance());
	HDGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	HDGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	HDGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	HDGameInstance->CharacterClass = SelectedSlot->GetCharacterClass();

	/*
	if (IsValid(SelectedSlot))
	{
		HDGameMode->TravelToMap(SelectedSlot);
	}*/
}

void UMVVM_LoadScreen::LoadData()
{
	AHorrorDungeonGameMode* HDGameMode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(HDGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = HDGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->InitializeSlot();
		
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		LoadSlot.Value->SetCharacterClass(SaveObject->CharacterClass);
		
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InLoadNumSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InLoadNumSlots);
}

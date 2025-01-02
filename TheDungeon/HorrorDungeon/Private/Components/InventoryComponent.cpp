// Copyright DM


#include "Components/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::AddItemToInventory(const FItemValues& NewItemValues)
{
	Inventory.Add(NewItemValues, NewItemValues.ItemObjectValues.ItemClass);
}





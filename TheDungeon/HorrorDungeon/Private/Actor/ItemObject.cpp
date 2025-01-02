// Copyright DM


#include "Actor/ItemObject.h"

bool UItemObject::IsSupportedForNetworking() const
{
	return true;
}

void UItemObject::Rotate()
{
	ItemObjectValues.bRotated = !ItemObjectValues.bRotated;
}

FIntPoint UItemObject::GetDimensions() const
{
	if (ItemObjectValues.bRotated)
	{
		return FIntPoint(ItemObjectValues.Dimensions.Y, ItemObjectValues.Dimensions.X);
	}
	return ItemObjectValues.Dimensions;
}

UMaterialInterface* UItemObject::GetIcon() const
{
	if (ItemObjectValues.bRotated)
	{
		return ItemObjectValues.IconRotated;
	}
	return ItemObjectValues.Icon;
}

TSubclassOf<AEquippableActor> UItemObject::GetItemClass() const
{
	return ItemObjectValues.ItemClass;
}

EItemType UItemObject::GetItemType() const
{
	return ItemObjectValues.ItemType;
}

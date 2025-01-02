// Copyright DM


#include "HUD/WidgetController/InventoryWidgetController.h"
#include "Interfaces/InventoryInterface.h"


UInventoryComponent* UInventoryWidgetController::GetInventoryComponent(AActor* Actor)
{
	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(Actor))
	{
		return InventoryInterface->GetInventoryComponent();
	}
	else
	{
		return nullptr;
	}
}

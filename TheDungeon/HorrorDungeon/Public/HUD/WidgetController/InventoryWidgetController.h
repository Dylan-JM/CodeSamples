// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/HDWidgetController.h"
#include "InventoryWidgetController.generated.h"

class ABaseCharacter;
class UInventoryComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class HORRORDUNGEON_API UInventoryWidgetController : public UHDWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryComponent* GetInventoryComponent(AActor* Actor);
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"


class UItemObject;
class UInventoryComponent;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORDUNGEON_API IInventoryInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetInventoryComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetInsuranceInventoryComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetActorName();
	
};

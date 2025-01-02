// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/ItemObject.h"
#include "InventoryComponent.generated.h"


class AArmor;
class UItemObject;
class AEquippableActor;

USTRUCT(BlueprintType, Blueprintable)
struct FItemValues
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 TopLeftIndex;
	
	UPROPERTY(BlueprintReadWrite)
	FItemObjectValues ItemObjectValues;

	// Equality comparison operator
	bool operator==(const FItemValues& Other) const
	{
		return TopLeftIndex == Other.TopLeftIndex && ItemObjectValues == Other.ItemObjectValues;
	}

	// Hash function
	friend uint32 GetTypeHash(const FItemValues& ItemValues)
	{
		uint32 Hash = 0;
		HashCombine(Hash, ItemValues.TopLeftIndex);
		HashCombine(Hash, GetTypeHash(ItemValues.ItemObjectValues));
		return Hash;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class HORRORDUNGEON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 Columns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constants")
	int32 Rows;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame)
	TMap<FItemValues, TSubclassOf<AEquippableActor>> Inventory;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<UItemObject*> Items;

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const FItemValues& NewItemValues);


private:

	
};

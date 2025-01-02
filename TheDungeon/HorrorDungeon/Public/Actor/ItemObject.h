// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "EquippableActor.h"
#include "GlobalEnums.h"
#include "ItemObject.generated.h"

class AEquippableActor;
class ABaseWeapon;
class UMaterialInterface;
/**
 * 
 */

USTRUCT(BlueprintType, Blueprintable)
struct FItemObjectValues
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* Icon;
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* IconRotated;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AEquippableActor> ItemClass;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Dimensions;

	UPROPERTY(BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite)
	bool bRotated = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsTwoHanded = false;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxStacks = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStacks = 1;

	// Equality comparison operator
	bool operator==(const FItemObjectValues& Other) const
	{
		return Icon == Other.Icon &&
			   IconRotated == Other.IconRotated &&
			   ItemClass == Other.ItemClass &&
			   Dimensions == Other.Dimensions &&
			   ItemType == Other.ItemType &&
			   bRotated == Other.bRotated &&
			   bIsTwoHanded == Other.bIsTwoHanded &&
			   	MaxStacks == Other.MaxStacks &&
				CurrentStacks == Other.CurrentStacks;
		
			
	}
};

inline uint32 GetTypeHash(const FItemObjectValues& ItemObjectValues)
{
	uint32 Hash = 0;
	HashCombine(Hash, GetTypeHash(ItemObjectValues.Icon));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.IconRotated));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.ItemClass));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.Dimensions));
	HashCombine(Hash, GetTypeHash(static_cast<uint8>(ItemObjectValues.ItemType)));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.bRotated));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.bIsTwoHanded));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.MaxStacks));
	HashCombine(Hash, GetTypeHash(ItemObjectValues.CurrentStacks));
	return Hash;
}

UCLASS(Blueprintable, BlueprintType)
class HORRORDUNGEON_API UItemObject : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override;

	UFUNCTION(BlueprintCallable)
	void Rotate();

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions() const;

	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetIcon() const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AEquippableActor> GetItemClass() const;

	UFUNCTION(BlueprintCallable)
	EItemType GetItemType() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FItemObjectValues ItemObjectValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	bool Equipped;
	
};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "HDInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FHDInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FHDInputAction> AbilityInputActions;
};

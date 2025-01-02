// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalEnums.h"
#include "Interfaces/HighlightInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "PlayerCharacter/CharacterTypes.h"
#include "EquippableActor.generated.h"

class UBoxComponent;
class USphereComponent;
class UItemObject;
class ABaseCharacter;

UCLASS()
class HORRORDUNGEON_API AEquippableActor : public AActor, public IInventoryInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AEquippableActor();

	virtual void OnOwnerDead(const FVector& DeathImpulse);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void HandleCollision(bool bEnable);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UItemObject* GetDefaultItemObject();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetItemObjectVariables();

	void CreateMyClass();
	void CleanupSubObject();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> HighlightCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn = "true"), Category="ItemObjectVariables")
	UItemObject* ItemObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName DefaultSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AltSocketName;
	
	/* Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Interface */

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	bool bOnGround = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 CurrentStacks = 1;

	UPROPERTY(EditDefaultsOnly)
	ECombatStyle CombatStyle;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABaseCharacter> OwningCharacter;

public:	
	FORCEINLINE UStaticMeshComponent* GetItemMesh() { return ItemMesh; }
	
};

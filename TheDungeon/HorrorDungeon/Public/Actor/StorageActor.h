// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HighlightInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "StorageActor.generated.h"

class UInventoryComponent;

UCLASS()
class HORRORDUNGEON_API AStorageActor : public AActor, public IInventoryInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AStorageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	
	/* Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override;
	virtual FText GetActorName_Implementation() override;
	/* End Interface */

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActorName;
	
protected:
	virtual void BeginPlay() override;



};

// Copyright DM


#include "Actor/StorageActor.h"

#include "Components/InventoryComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"

AStorageActor::AStorageActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AStorageActor::HighlightActor()
{
	ItemMesh->SetRenderCustomDepth(true);
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AStorageActor::UnHighlightActor()
{
	ItemMesh->SetRenderCustomDepth(false);
}

UInventoryComponent* AStorageActor::GetInventoryComponent_Implementation()
{
	return InventoryComponent;
}

FText AStorageActor::GetActorName_Implementation()
{
	return ActorName;
}


void AStorageActor::BeginPlay()
{
	Super::BeginPlay();
	
}




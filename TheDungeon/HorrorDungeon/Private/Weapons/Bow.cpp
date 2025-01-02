// Copyright DM


#include "Weapons/Bow.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"

ABow::ABow()
{
	BowMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("BowMesh");
	SetRootComponent(BowMeshComponent);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArrowMesh");
	ArrowMesh->SetupAttachment(GetRootComponent());
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowMesh->SetVisibility(false);

	FireComponent = CreateDefaultSubobject<UNiagaraComponent>("FireComponent");
	FireComponent->SetupAttachment(ArrowMesh);
	FireComponent->SetAutoActivate(false);
	
	HighlightCollision->SetupAttachment(GetRootComponent());
	WeaponCapsule->SetupAttachment(GetRootComponent());

}

bool ABow::HasArrow_Implementation()
{
	return false;
}

void ABow::HighlightActor()
{
	if (bOnGround)
	{
		BowMeshComponent->SetRenderCustomDepth(true);
		BowMeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void ABow::UnHighlightActor()
{
	BowMeshComponent->SetRenderCustomDepth(false);
}

void ABow::BeginPlay()
{
	Super::BeginPlay();
	if (OwningCharacter)
	{
		if (HasArrow())
		{
			ArrowMesh->SetVisibility(true);
		}
	}
}

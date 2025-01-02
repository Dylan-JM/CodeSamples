// Copyright DM


#include "Actor/EquippableActors/Armor.h"
#include "Components/BoxComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"

AArmor::AArmor()
{
	ArmorMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("ArmorMesh");
	SetRootComponent(ArmorMeshComponent);

	HighlightCollision->SetupAttachment(GetRootComponent());
}

void AArmor::HighlightActor()
{
	if (bOnGround)
	{
		ArmorMeshComponent->SetRenderCustomDepth(true);
		ArmorMeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AArmor::UnHighlightActor()
{
	ArmorMeshComponent->SetRenderCustomDepth(false);
}

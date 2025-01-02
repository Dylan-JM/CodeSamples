// Copyright DM


#include "Actor/EquippableActor.h"

#include "Actor/ItemObject.h"
#include "Components/BoxComponent.h"
#include "Engine/ActorChannel.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter/BaseCharacter.h"


AEquippableActor::AEquippableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	//bReplicates = true; 
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	RootComponent = ItemMesh;

	HighlightCollision = CreateDefaultSubobject<UBoxComponent>("HighlightCollision");
	HighlightCollision->SetupAttachment(RootComponent);
	HighlightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HighlightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HighlightCollision->SetCollisionResponseToChannel(ECC_Target, ECR_Block);

	bReplicateUsingRegisteredSubObjectList = true;
}

void AEquippableActor::OnOwnerDead(const FVector& DeathImpulse)
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetEnableGravity(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ItemMesh->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	
	SetLifeSpan(5.f);
}

void AEquippableActor::SetItemObjectVariables_Implementation()
{
}

void AEquippableActor::CreateMyClass()
{
	CleanupSubObject();

	ItemObject = NewObject<UItemObject>();
	AddReplicatedSubObject(ItemObject);
}

void AEquippableActor::CleanupSubObject()
{
	if (ItemObject)
	{
		RemoveReplicatedSubObject(ItemObject);
	}
}

void AEquippableActor::HighlightActor()
{
	if (bOnGround)
	{
		ItemMesh->SetRenderCustomDepth(true);
		ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AEquippableActor::UnHighlightActor()
{
	GetItemMesh()->SetRenderCustomDepth(false);
}

void AEquippableActor::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacter = Cast<ABaseCharacter>(GetOwner());

	CreateMyClass();
}

void AEquippableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEquippableActor, bOnGround);
}

void AEquippableActor::HandleCollision(bool bEnable)
{
	
}


UItemObject* AEquippableActor::GetDefaultItemObject_Implementation()
{
	return nullptr;
}



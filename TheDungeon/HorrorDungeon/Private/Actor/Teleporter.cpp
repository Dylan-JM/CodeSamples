// Copyright DM


#include "Actor/Teleporter.h"

#include "Components/DecalComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter/PlayerCharacter.h"

ATeleporter::ATeleporter()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Light, ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SetRootComponent(ItemMesh);

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircleDecal"));
	MagicCircleDecal->SetupAttachment(GetRootComponent());

	Light = CreateDefaultSubobject<UChildActorComponent>("Light");
	Light->SetupAttachment(GetRootComponent());
	
	bReplicates = true;
}

void ATeleporter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATeleporter, bStartTeleporting);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	MeshDynamicMaterial = UMaterialInstanceDynamic::Create(ItemMesh->GetMaterial(0), this);
	TeleportDecalMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
	ItemMesh->SetMaterial(0, MeshDynamicMaterial);
}

void ATeleporter::HighlightActor()
{
	ItemMesh->SetRenderCustomDepth(true);
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATeleporter::UnHighlightActor()
{
	ItemMesh->SetRenderCustomDepth(false);
}

void ATeleporter::ServerStartTeleportSequence_Implementation(FName InMapName)
{
	MapName = InMapName;
	FTimerHandle TeleportTimer;
	FTimerDelegate TeleportDelegate;
	TeleportDelegate.BindUObject(this, &ATeleporter::TeleportToMap);
	GetWorldTimerManager().SetTimer(TeleportTimer, TeleportDelegate, TeleportTime, false);
}

void ATeleporter::MultiCastStartTeleportAnimation_Implementation()
{
	StartTeleportAnimation();
	MagicCircleDecal->SetDecalMaterial(TeleportDecalMaterial);
	bStartTeleporting = true;
}

void ATeleporter::TeleportToMap()
{
	TArray<AActor*> ActorsToTeleport;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerCharacter::StaticClass(), ActorsToTeleport);
	for (AActor* Actor : ActorsToTeleport)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
		PlayerCharacter->ClientSaveProgress();
	}
	if (GetWorld())
	{
		GetWorld()->ServerTravel(FString::Printf(TEXT("/Game/HorrorDungeon/Maps/%s?listen"), *MapName.ToString()));
	}
}



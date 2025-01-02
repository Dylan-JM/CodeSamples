// Copyright DM


#include "Lighting/HDTorch.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "Lighting/HDLight.h"


AHDTorch::AHDTorch()
{
	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>("TorchMesh");
	SetRootComponent(TorchMesh);
	
	Light = CreateDefaultSubobject<UChildActorComponent>("Light");
	Light->SetupAttachment(GetRootComponent());
	
	FireComponent = CreateDefaultSubobject<UNiagaraComponent>("FireEffect");
	FireComponent->SetupAttachment(GetRootComponent());

	HighlightCollision = CreateDefaultSubobject<UBoxComponent>("HighlightCollision");
	HighlightCollision->SetupAttachment(GetRootComponent());
	HighlightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HighlightCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HighlightCollision->SetCollisionResponseToChannel(ECC_Target, ECR_Block);
	
}

void AHDTorch::ToggleLight(bool bEnable)
{
	if (HDLight == nullptr) return;
	if (bEnable)
	{
		HDLight->PointLight.Get()->SetIntensity(LightIntensity);
		HDLight->PointLight.Get()->SetAttenuationRadius(1000.f);
		FireComponent->Activate();
	}
	else
	{
		HDLight->PointLight.Get()->SetIntensity(0.f);
		HDLight->PointLight.Get()->SetAttenuationRadius(0.f);
		FireComponent->Deactivate();
	}
}

void AHDTorch::HighlightActor()
{
	TorchMesh->SetRenderCustomDepth(true);
	TorchMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AHDTorch::UnHighlightActor()
{
	TorchMesh->SetRenderCustomDepth(false);
}

void AHDTorch::BeginPlay()
{
	Super::BeginPlay();
	HDLight = Cast<AHDLight>(Light->GetChildActor());
	if (HDLight)
	{
		HDLight->PointLight.Get()->SetIntensity(0.f);
		HDLight->PointLight.Get()->SetAttenuationRadius(0.f);
	}

}

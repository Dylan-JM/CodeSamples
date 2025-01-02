// Copyright DM

#include "Actor/EquippableActors/HandHeldTorch.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "Lighting/HDLight.h"

AHandHeldTorch::AHandHeldTorch()
{
	Light = CreateDefaultSubobject<UChildActorComponent>("Light");
	Light->SetupAttachment(GetRootComponent());

	FireComponent = CreateDefaultSubobject<UNiagaraComponent>("FireEffect");
	FireComponent->SetupAttachment(GetRootComponent());
}

void AHandHeldTorch::ToggleLight(bool bEnable)
{
	if (HDLight == nullptr) return;
	if (bEnable)
	{
		if (HDLight->PointLight)
		{
			HDLight->PointLight.Get()->SetIntensity(LightIntensity);
			HDLight->PointLight.Get()->SetAttenuationRadius(1000.f);
		}
		if (FireComponent)
		{
			FireComponent->Activate();
		}
	}
	else
	{
		if (HDLight->PointLight)
		{
			HDLight->PointLight.Get()->SetIntensity(0.f);
			HDLight->PointLight.Get()->SetAttenuationRadius(0.f);
		}
		if (FireComponent)
		{
			FireComponent->Deactivate();
		}
	}
}

void AHandHeldTorch::BeginPlay()
{
	Super::BeginPlay();
	HDLight = Cast<AHDLight>(Light->GetChildActor());
}


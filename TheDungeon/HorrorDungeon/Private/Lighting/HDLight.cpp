
#include "Lighting/HDLight.h"

#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "PlayerCharacter/BaseCharacter.h"

AHDLight::AHDLight()
{
	PrimaryActorTick.bCanEverTick = true;
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	SetRootComponent(PointLight);
	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);

	TriggerVolume->SetCollisionObjectType(ECC_Light);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
}


void AHDLight::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerVolume)
	{
		if (PointLight)
		{
			TriggerVolume->SetSphereRadius(PointLight->AttenuationRadius);
		}
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AHDLight::OnSphereOverlap);
	}
}

void AHDLight::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ABaseCharacter> BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		BaseCharacter->InLight = true;
	}
}


void AHDLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


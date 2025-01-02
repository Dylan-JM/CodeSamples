
#include "Components/LightingComponent.h"
#include "Components/PointLightComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "Lighting/HDLight.h"
#include "PlayerCharacter/BaseCharacter.h"


ULightingComponent::ULightingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULightingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Character == nullptr) Character = Cast<ACharacter>(GetOwner());

	
}

float ULightingComponent::GetLightingAmount()
{
	if (Character == nullptr) Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) return 0;
	TArray<AActor*> OverlappingActors;
	Character->GetOverlappingActors(OverlappingActors, AHDLight::StaticClass());
	float NearestActorDistance = 100000.f;
	TObjectPtr<AActor> NearestActor;
	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		float DistanceToActor = Character->GetDistanceTo(OverlappingActors[i]);
		if (DistanceToActor < NearestActorDistance)
		{
			NearestActorDistance = DistanceToActor;
			NearestActor = OverlappingActors[i];
		}
	}
	if (NearestActor)
	{
		FVector Loc = Character->GetActorLocation();
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("LightTrace")), true, Character);
		float Result = 0;
		TObjectPtr<AHDLight> NearestLight = Cast<AHDLight>(NearestActor);
		if (NearestLight && NearestLight->PointLight)
		{
			FVector End = NearestLight->GetActorLocation();
			float Distance = FVector::Dist(Loc,End);
			float LightRadius = NearestLight->PointLight->AttenuationRadius;
			bool bHit = GetWorld()->LineTraceTestByChannel(Loc,End, ECC_Light, Params);
			if (Distance <= LightRadius && !bHit)
			{
				Result = FMath::Pow(FMath::Max(0.0f, 1.0f - (Distance / LightRadius)), (NearestLight->PointLight->LightFalloffExponent + 1));
				Result = FMath::Clamp(Result, 0.0f, 1.0f);
			}
			else
			{
				Result = 0.0f;
			}
			
			if (Result > 1.0f)
			{
				Result = 1.0f;
			}
		}
		Result *= 200.f;
		Result = FMath::Clamp(Result, 0.0f, 100.0f);
		//FString ResultString = FString::SanitizeFloat(Result);
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, ResultString);
		return Result;
	}
	return 0;
}



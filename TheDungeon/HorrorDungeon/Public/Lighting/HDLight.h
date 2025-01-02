
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HDLight.generated.h"

class USphereComponent;
class UPointLightComponent;

UCLASS()
class HORRORDUNGEON_API AHDLight : public AActor
{
	GENERATED_BODY()
	
public:	
	AHDLight();
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> PointLight;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/* Variables */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> TriggerVolume;


private:


public:
	
};

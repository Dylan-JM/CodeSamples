// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Actor/EquippableActor.h"
#include "HandHeldTorch.generated.h"

class UNiagaraComponent;
class AHDLight;

UCLASS()
class HORRORDUNGEON_API AHandHeldTorch : public AEquippableActor
{
	GENERATED_BODY()


public:

	AHandHeldTorch();
	
	UFUNCTION(BlueprintCallable)
	void ToggleLight(bool bEnable);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> Light;


protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float LightIntensity;
private:

	TObjectPtr<AHDLight> HDLight;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TorchMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> FireComponent;

};

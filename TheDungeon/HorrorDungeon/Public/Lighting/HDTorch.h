// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HighlightInterface.h"
#include "HDTorch.generated.h"


class UBoxComponent;
class UNiagaraComponent;
class AHDLight;

UCLASS()
class HORRORDUNGEON_API AHDTorch : public AActor, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AHDTorch();

	UFUNCTION(BlueprintCallable)
	void ToggleLight(bool bEnable);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> Light;

	/* Highlight Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HighlightCollision;
	

};

// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HighlightInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "Teleporter.generated.h"

class UMainHUD;

UCLASS()
class HORRORDUNGEON_API ATeleporter : public AActor, public IHighlightInterface
{
	GENERATED_BODY()
	
public:	
	ATeleporter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/* Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Interface */

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastStartTeleportAnimation();

	UFUNCTION(Server, Reliable)
	void ServerStartTeleportSequence(FName InMapName);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartTeleportAnimation();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainHUD> TeleportMenu;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bStartTeleporting = false;
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UFUNCTION(BlueprintCallable)
	void TeleportToMap();

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* MeshDynamicMaterial;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* TeleportDecalMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	TObjectPtr<UDecalComponent> MagicCircleDecal;

	UPROPERTY(EditAnywhere)
	float TeleportTime = 10.f;
private:

	FName MapName;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DecalMaterial;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> Light;
};

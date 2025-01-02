// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Bow.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API ABow : public ABaseWeapon
{
	GENERATED_BODY()

public:

	ABow();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool HasArrow();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> BowMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> FireComponent;
	
	/* Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Interface */

protected:

	virtual void BeginPlay() override;
};

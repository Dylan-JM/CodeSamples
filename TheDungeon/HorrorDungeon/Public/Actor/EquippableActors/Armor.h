// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Actor/EquippableActor.h"
#include "Armor.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API AArmor : public AEquippableActor
{
	GENERATED_BODY()


public:

	AArmor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Armor)
	TObjectPtr<USkeletalMeshComponent> ArmorMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category= Armor)
	TObjectPtr<USkeletalMesh> ArmorMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float ArmorIncrease;

	/* Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Interface */
};

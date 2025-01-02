// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "MeleeWeapon.generated.h"


/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:

	AMeleeWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> AbilityWeaponBox;

	UPROPERTY()
	TArray<AActor*> AbilityOverlappingActors;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AbilityActorsToIgnore;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnAbilityBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void AbilityHandleCollision(bool bEnable);


};

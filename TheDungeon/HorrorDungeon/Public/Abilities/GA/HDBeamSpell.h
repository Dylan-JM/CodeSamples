// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA/HDDamageGameplayAbility.h"
#include "HDBeamSpell.generated.h"


class ABaseWeapon;
/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDBeamSpell : public UHDDamageGameplayAbility
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void StoreDataInfo();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerStoreDataInfo(FVector TempHitLocation, AActor* TempHitActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStoreDataInfo(FVector TempHitLocation, AActor* TempHitActor);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

protected:

	UPROPERTY(BlueprintReadWrite, Category= "Beam")
	FVector HitLocation;

	UPROPERTY(BlueprintReadWrite, Category= "Beam")
	TObjectPtr<AActor> HitActor;

	UPROPERTY(EditDefaultsOnly, Category= "Beam")
	int32 MaxNumShockTargets = 5;
};

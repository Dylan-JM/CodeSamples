// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "Actor/HDProjectile.h"
#include "HDFireBall.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API AHDFireBall : public AHDProjectile
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult) override;

	virtual void OnHit() override;

	
};

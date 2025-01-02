// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "PlayerCharacter/BaseCharacter.h"
#include "HealthCharacter.generated.h"

class APlayerCharacter;
class UMainHUD;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);

UCLASS()
class HORRORDUNGEON_API AHealthCharacter : public ABaseCharacter
{
	GENERATED_BODY()


public:

	AHealthCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Die(const FVector& DeathImpulse, bool bRagdoll = true) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayerAdded(APlayerCharacter* PlayerCharacter);
	UFUNCTION()
	void PlayerDied(AActor* DeadPlayer);
	UFUNCTION()
	void PlayerRevived();


	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanRevive = true;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDied PlayerDiedDelegate;

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPlayerDied();
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> HealthDepletion;
	
};

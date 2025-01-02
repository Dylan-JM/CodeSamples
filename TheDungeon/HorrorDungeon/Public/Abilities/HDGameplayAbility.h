// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewInputAbilityTest.h"
#include "Abilities/GameplayAbility.h"
#include "HDGameplayAbility.generated.h"

class APlayerCharacter;

UCLASS()
class HORRORDUNGEON_API UHDGameplayAbility : public UNewInputAbilityTest
{
	GENERATED_BODY()

public:
	UHDGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages, FGameplayTag MontageTag) const;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomMontageWithTag(const TArray<FTaggedMontage>& TaggedMontages, FGameplayTag MontageTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTagContainer GetAbilityTags() { return AbilityTags; }

	UFUNCTION(BlueprintCallable)
	bool IsCorrectWeapon(TSubclassOf<ABaseWeapon> Weapon);
protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

	UPROPERTY(BlueprintReadWrite, Category= "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category= "Beam")
	TObjectPtr<APlayerCharacter> OwnerCharacter;
};

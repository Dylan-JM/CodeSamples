// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Abilities/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class AEquippableActor;
class ABaseCharacter;
class ABaseWeapon;
class UAbilitySystemComponent;
class UNiagaraSystem;
class UAnimMontage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*Damage Amount */);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;
	
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class HORRORDUNGEON_API ICombatInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableWeaponCollision(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableWeaponCollision();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ABaseWeapon* GetEquippedWeapon1();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ABaseWeapon* GetEquippedOffhandWeapon();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	
	virtual void Die(const FVector& DeathImpulse, bool bRagdoll = true) = 0;

	virtual FOnDeath& GetOnDeathDelegate() = 0;
	virtual FOnDamageSignature& GetOnDamageSignature() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAbilityMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool InLoop);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStartShock(bool IsShock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetStepping(bool bStepping);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsAttacking(bool InIsAttacking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ReserveAttackToken(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnAttackToken(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool OnSameTeam(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBlocking(bool bInBlock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsBlocking();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetBlockingMultiplier();
};

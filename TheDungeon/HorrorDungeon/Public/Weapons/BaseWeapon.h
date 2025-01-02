// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "HDAbilityTypes.h"
#include "Actor/EquippableActor.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CombatInterface.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;
class UNiagaraSystem;
class UAnimMontage;
class UGameplayEffect;
class UCapsuleComponent;

UCLASS()
class HORRORDUNGEON_API ABaseWeapon : public AEquippableActor
{
	GENERATED_BODY()
	
public:
	
	ABaseWeapon();

	virtual void OnOwnerDead(const FVector& DeathImpulse) override;
	
	virtual void HandleCollision(bool bEnable) override;
	
	void EmptySameActorArray();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	TObjectPtr<UCapsuleComponent> WeaponCapsule;
	
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MeleeDamageMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposedOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	FDamageEffectParams DebuffParams;
		
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly)
	float DamageConsumptionMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> DefaultAbility;
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditDefaultsOnly, Category= "Combat");
	FGameplayTag GameplayCueTag;

public:
	FORCEINLINE TArray<AActor*> GetActorsToIgnore() { return ActorsToIgnore; }
	FORCEINLINE UCapsuleComponent* GetWeaponCapsule() { return WeaponCapsule; }
	
};

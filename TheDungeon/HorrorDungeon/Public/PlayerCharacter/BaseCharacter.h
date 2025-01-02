
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterTypes.h"
#include "PlayerState/BasePlayerState.h"
#include "Abilities/Data/CharacterClassInfo.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/HighlightInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "BaseCharacter.generated.h"

class AHDEffectActor;
class ABaseEnemy;
class AEquippableActor;
class AArmor;
class UInventoryComponent;
class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class ABaseWeapon;
class UAnimMontage;
class UCombatComponent;
class UGameplayEffect;
class UGameplayAbility;
class UHDAbilitySystemComponent;
class UHDAttributeSet;
class ULightingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABaseCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityFinished);


UCLASS()
class HORRORDUNGEON_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IInventoryInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/* GAS */
	void HandleGameplayEffectRemoved(const FActiveGameplayEffectHandle InGEHandle);
	UFUNCTION()
	void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InGERemovalInfo);

	void UseManaPotion();
	int32 CurrentManaPotions;
	int32 MaxNumOfManaPotions = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHDEffectActor> ManaPotionClass;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse, bool bRagdoll = true);
	
	UPROPERTY(BlueprintAssignable, Category= "Abilities")
	FCharacterDiedDelegate OnCharacterDied;

	FOnASCRegistered OnAscRegistered;
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityFinished AttackFinishedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityFinished BlockFinishedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityFinished TeleportFinishedDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityFinished HealFinishedDelegate;
	
	UPROPERTY(EditAnywhere)
	EAttackType AttackType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly)
	bool bUseRotationYawWhenAiming = true;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldHoldBowString = true;
	
	/* Light Mechanics */
	bool InLight = false;
	/* */

	/* Weapons and Amulets*/

	void SetCombatStyle(ECombatStyle InCombatStyle);

	UPROPERTY(Replicated)
	ECombatStyle CombatStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<AEquippableActor> Weapon1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<AEquippableActor> OffhandItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, Replicated)
	TObjectPtr<ABaseWeapon> EquippedWeapon1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	TObjectPtr<AEquippableActor> EquippedOffhandItem;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	TObjectPtr<AEquippableActor> EquippedAmulet1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<AEquippableActor> StartingAmulet1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	TObjectPtr<AEquippableActor> EquippedAmulet2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<AEquippableActor> StartingAmulet2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	TObjectPtr<AEquippableActor> EquippedAmulet3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<AEquippableActor> StartingAmulet3;
	/* */

	/* Armor and Inventory*/

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Armor)
	TSubclassOf<AEquippableActor> StartingHelmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Armor, ReplicatedUsing=OnRep_Helmet)
	TObjectPtr<AArmor> Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Armor)
	TSubclassOf<AEquippableActor> StartingChestplate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Armor, ReplicatedUsing=OnRep_Chestplate)
	TObjectPtr<AArmor> Chestplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Armor)
	TSubclassOf<AEquippableActor> StartingGauntlets;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Armor, ReplicatedUsing=OnRep_Gauntlets)
	TObjectPtr<AArmor> Gauntlets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Armor)
	TSubclassOf<AEquippableActor> StartingLegs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Armor, ReplicatedUsing=OnRep_Legs)
	TObjectPtr<AArmor> Legs;

	UPROPERTY(VisibleAnywhere, Category= Armor)
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;
	
	UPROPERTY(VisibleAnywhere, Category= Armor)
	TObjectPtr<USkeletalMeshComponent> ChestplateMesh;

	UPROPERTY(VisibleAnywhere, Category= Armor)
	TObjectPtr<USkeletalMeshComponent> GauntletsMesh;

	UPROPERTY(VisibleAnywhere, Category= Armor)
	TObjectPtr<USkeletalMeshComponent> LegsMesh;

	/* End Armor and inventory */

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsBlocking = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddEquippedItemsToInventory();


	// Movement 
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsAiming = false;
	
	void ChangeMovementSpeed(bool InAiming); 

	UPROPERTY(EditAnywhere, Category=Combat)
	float CombatWalkSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	float SprintSpeed = 600.f;
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bDeadOnGround = false;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bDead = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;


	/* Weapon Properties */

	UPROPERTY(BlueprintReadWrite)
	bool bUseFireArrow = false;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	virtual void SpawnWeapons();
	
	/* GAS */
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	UFUNCTION(BlueprintCallable)
	virtual void InitializeDefaultAttributes() const;

	UFUNCTION(BlueprintCallable)
	void AddCharacterAbilities();
	
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> StaminaRegeneration;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UAttributeSet> AttributeSetBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActorName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animations")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	friend UHDAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULightingComponent> LightingComponent;

	/* Death */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;

	UFUNCTION(BlueprintCallable)
	void DeathMontageEnded();

	/* End Death */
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float LifeSpan = 5.f;

	/* Minion */
	int32 MinionCount = 0;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
	
	UPROPERTY(EditAnywhere)
	int32 TeamNumber = 1;

private:
	
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

	UPROPERTY(EditDefaultsOnly)
	int32 AttackTokensCount = 1;


public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSetBase; }
	
	/* Combat Interface */
	virtual void EnableWeaponCollision_Implementation(const FGameplayTag& MontageTag) override;
	virtual void DisableWeaponCollision_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse, bool bRagdoll = true) override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAbilityMontages_Implementation() override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual ABaseWeapon* GetEquippedWeapon1_Implementation() override;
	virtual ABaseWeapon* GetEquippedOffhandWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	virtual bool GetIsAttacking_Implementation() override;
	virtual void SetIsAttacking_Implementation(bool InIsAttacking) override;
	virtual bool ReserveAttackToken_Implementation(int32 Amount) override;
	virtual void ReturnAttackToken_Implementation(int32 Amount) override;
	virtual int32 GetTeamNumber(AActor* Actor);
	virtual bool OnSameTeam_Implementation(AActor* Actor) override;
	virtual void SetIsBlocking_Implementation(bool bInBlock) override;
	virtual bool GetIsBlocking_Implementation() override;
	virtual float GetBlockingMultiplier_Implementation() override;
	/* End Combat Interface */

	/** Inventory Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override;
	virtual FText GetActorName_Implementation() override;
	/** end Inventory Interface */

	UFUNCTION()
	void OnRep_Helmet();
	UFUNCTION()
	void OnRep_Chestplate();
	UFUNCTION()
	void OnRep_Gauntlets();
	UFUNCTION()
	void OnRep_Legs();
};


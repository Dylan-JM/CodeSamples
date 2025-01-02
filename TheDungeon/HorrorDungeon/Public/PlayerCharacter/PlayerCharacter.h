// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interfaces/PlayerInterface.h"
#include "GlobalEnums.h"
#include "Actor/EquippableActors/Armor.h"
#include "PlayerCharacter.generated.h"

class AHealthCharacter;
class ABaseEnemy;
class AEquippableActor;
class UNiagaraComponent;
enum class ECharacterAbilityInputID : uint8;
enum class EActionState : uint8;
class ABasePlayerController;
class UTargetSystemComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class IEnemyInterface;
class USaveGame;
class ULoadScreenSaveGame;
class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRevived);

UCLASS()
class HORRORDUNGEON_API APlayerCharacter : public ABaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void Jump() override;
	
	UFUNCTION(BlueprintCallable)
	void TraceUnderCrosshairs(FHitResult& TraceHitResult, float TraceDistance, bool bUseVisibilityChannel = false);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<AEquippableActor> ItemToEquip, EItemType ItemType, int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerEquipItem(TSubclassOf<AEquippableActor> ItemToEquip, EItemType ItemType, int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void UnequipItem(EItemType ItemType, int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerUnequipItem(EItemType ItemType, int32 SlotIndex);
	
	/* Movement mechanics*/
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector2D MovementDirectionVector;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsStepping;

	UPROPERTY(Replicated)
	bool bIsSprinting = false;
	/* End Movement Mechanics */
	
	/* Anim Montages */
	UPROPERTY(EditAnywhere, Category=Animations)
	TArray<UAnimMontage*> Attacks;
	UPROPERTY(EditAnywhere, Category=Animations)
	TObjectPtr<UAnimMontage> LeftSideStepMontage;
	UPROPERTY(EditAnywhere, Category=Animations)
	TObjectPtr<UAnimMontage> RightSideStepMontage;
	/* End Anim Montages */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABasePlayerController> PlayerCharacterController;

	float TotalArmor;
	float BaseArmor;
	
	/* Highlighting Enemies */
	IHighlightInterface* LastActor;
	
	IHighlightInterface* ThisActor;
	
	UFUNCTION(BlueprintCallable)
	UObject* GetThisActor() const;

	/* */
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    TArray<APawn*> MeleeAttackers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxNumOfMeleeAttackers = 3;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<APawn*> RangedAttackers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxNumOfRangedAttackers = 3;


	/* Save Game */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EquipStartingItems();
	
	UFUNCTION(BlueprintCallable)
	void ClientSaveProgress();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddSavedItemsToInventory();

	/* Inventory */


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InsuranceInventoryComponent;
	
	UFUNCTION(BlueprintCallable)
	void SpawnItemFromActor(TSubclassOf<AEquippableActor> ActorToSpawn, bool GroundClamp, int32 InStacks);

	/**/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPlayerRevived OnPlayerRevived;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	float DefaultTraceDistance = 100.f;
private:
	void UpdateLightVignette();

	/* Camera */
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/* Aiming */
	void InterpFOV(float DeltaTime);
	float DefaultFOV;
	UPROPERTY(EditAnywhere, Category=Aiming)
	float ZoomedFOV = 30.f;
	UPROPERTY(EditAnywhere, Category=Aiming)
	float ZoomInterpSpeed = 20.f;
	float CurrentFOV;

	/**/
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastLevelUpParticles() const;

	/* Save Game */
	UFUNCTION(Client, Reliable)
	void SaveInGameProgressData();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientLoadProgress();
	
	UFUNCTION(Server, Reliable)
	void ServerLoadProgress(int32 InLevel, int32 InXP, int32 InAttributePoints, int32 InSpellPoints);
	
	UFUNCTION(Server, Reliable)
	void ServerLoadDefaultAttributes(float InStrength, float InInt, float InRes, float InVigor);

	UFUNCTION(Server, Reliable)
	void ServerAddCharacterAbilitiesFromClientSaveData(const TArray<FSavedAbility>& InSavedAbilities);
	
	UFUNCTION(Server, Reliable)
	void ServerFirstTimeLoadIn();

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex);
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ULoadScreenSaveGame* PlayerSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category= "Ability Info")
	TObjectPtr<UAbilityInfo> PlayerAbilityInfo;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	/* End Save Game */
	
	UPROPERTY()
	TObjectPtr<AHealthCharacter> HealthCharacter;
public:
	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributesPointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;

	virtual UInventoryComponent* GetInsuranceInventoryComponent_Implementation() override;
	/* End Player Interface */
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void SetStepping_Implementation(bool bStepping) override;
	virtual void Die(const FVector& DeathImpulse, bool bRagdoll = true) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EmptyInventory();
	/* End Combat Interface */
	
	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;

	FTimerHandle DeathTimer;
	
	void SetAiming(bool bInIsAiming);
	UFUNCTION(Server,Reliable)
	void ServerSetAiming(bool bInIsAiming);

	void SetSprinting(bool bInIsSprinting);
	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(bool bInIsSprinting);

	
	/* Camera Getters */
	UFUNCTION(BlueprintCallable)
	class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/* End Camera Getters */
	
	UFUNCTION(BlueprintCallable)
	FHitResult GetTraceResult(float TraceDistance, bool bUseVisibilityChannel = false);

	UFUNCTION(BlueprintCallable)
	FVector GetImpactPoint();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UPROPERTY(Replicated)
	FVector ImpactPoint;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetImpactPoint(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastSetImpactPoint(const FVector_NetQuantize& TraceHitTarget);

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;


	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"


class ABlasterGameMode;
class UNiagaraComponent;
class UNiagaraSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

class ULagCompensationComponent;
class UBoxComponent;
class UBuffComponent;
class ABlasterPlayerState;
class USoundCue;
class ABlasterPlayerController;
class UCombatComponent;
class UWidgetComponent;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class AWeapon;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();
	// Play montages
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlayThrowGrenadeMontage();
	void PlaySwapMontage();
	virtual void OnRep_ReplicatedMovement() override;
	void Elim(bool bPlayerLeftGame);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim(bool bPlayerLeftGame);
	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void SpawnDefaultWeapon();

	UPROPERTY()
	TMap<FName, UBoxComponent*> HitCollisionBoxes;

	bool bFinishedSwapping = false;
	
	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();
	FOnLeftGame OnLeftGame;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();
	bool bActivated = false;

	void SetTeamColor(ETeam Team);
protected:

	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();
	void CalculateAO_Pitch();
	float CalculateSpeed();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	void PlayHitReactMontage();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void SetSpawnPoint();
	void OnPlayerStateInitialized();
	
	// Poll for any relevant classes and initialize HUD
	void PollInit();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> BlasterMappingContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EquipAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> StopFireAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> GrenadeAction;


	/* Reload anim notify fix */
	// Delegate function i bound to the OnMontageEnded delegate on the AnimInstance.
	UFUNCTION()
	void ReloadMontageEndedHandler(UAnimMontage* Montage, bool bInterrupted);
	// Timer we need if montage is interrupted
	FTimerHandle ReloadTimer;
	// What to do when timer is finished
	void ReloadTimerFinished();

	/* Hit boxes used for server-side rewind */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> head;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> pelvis;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> spine_02;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> spine_03;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> upperarm_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> upperarm_r;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> lowerarm_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> lowerarm_r;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hand_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hand_r;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> backpack;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> blanket;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> thigh_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> thigh_r;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> calf_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> calf_r;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> foot_l;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> foot_r;

	FTimerHandle RespawnTimer;
	
private:
	void RotateInPlace(float DeltaTime);
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	void HideCameraIfCharacterClose();
	void TurnInPlace(float DeltaTime);
	UFUNCTION()
	void OnRep_Health(float LastHealth);
	UFUNCTION()
	void OnRep_Shield(float LastShield);
	void ElimTimerFinished();

	bool bLeftGame = false;
	
	UPROPERTY()
	TObjectPtr<ABlasterGameMode> BlasterGameMode;
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	ETurningInPlace TurningInPlace;
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	float ProxyYaw;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float TimeSinceLastMovementReplication;
	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, Category="Player Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxShield = 100.f;
	UPROPERTY(ReplicatedUsing=OnRep_Shield, EditAnywhere, Category="Player Stats")
	float Shield = 0.f;

	/* Dissolve Effect */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	FOnTimelineFloat DissolveTrack;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DissolveCurve;
	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	// Material instance set on the blueprint, used with the dynamic material instance
	UPROPERTY(VisibleAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	/* Team Colours */
	UPROPERTY(EditAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> RedDissolveMatInst;
	UPROPERTY(EditAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> RedMaterial;
	UPROPERTY(EditAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> BlueDissolveMatInst;
	UPROPERTY(EditAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> BlueMaterial;
	
	UPROPERTY(EditAnywhere, Category=Elim)
	TObjectPtr<UMaterialInstance> OriginalMaterial;
	
	/* Elim effects */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ElimBotEffect;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> ElimBotComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ElimBotSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> CrownSystem;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> CrownComponent;
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> OverheadWidget;
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;
	/* Blaster Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatComponent> Combat;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBuffComponent> Buff;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULagCompensationComponent> LagCompensation;
	// Montages
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> ElimMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> ReloadMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> GrenadeMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> SwapMontage;
	
	UPROPERTY()
	TObjectPtr<ABlasterPlayerController> BlasterPlayerController;
	UPROPERTY()
	TObjectPtr<ABlasterPlayerState> BlasterPlayerState;

	/* Grenade */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AttachedGrenade;

	/* Default Weapon */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_YAW() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	TObjectPtr<AWeapon> GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	bool IsLocallyReloading();
	FORCEINLINE bool IsHoldingTheFlag() const;
	ETeam GetTeam();
	void SetHoldingTheFlag(bool bHolding);
};

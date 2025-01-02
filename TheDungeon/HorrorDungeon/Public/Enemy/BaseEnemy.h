
#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/OverlayWidgetController.h"
#include "PlayerCharacter/BaseCharacter.h"
#include "Interfaces/EnemyInterface.h"
#include "BaseEnemy.generated.h"


class APatrolRoute;
class UBehaviorTree;
class AEnemyAIController;
class UWidgetComponent;
class ABaseWeapon;

UCLASS()
class HORRORDUNGEON_API ABaseEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	FVector CalculateFutureActorLocation(AActor* Actor, float Time = 1.f);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChaseAttackTarget(AActor* AttackTarget);
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<APatrolRoute> PatrolRoute;


	/* AI Perception*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI Perception")
	float SightRadius = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI Perception")
	float LoseSightRadius = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI Perception")
	float HearingRange = 500.f;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	bool bCanBlockAttacks = false;
	/* End AI Perception */

	UPROPERTY(BlueprintReadWrite)
	bool StopUpdateVisibility = false;

	UPROPERTY(BlueprintReadWrite)
	bool bHasItem;
protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
		 
	UPROPERTY(EditAnywhere, Category=Lighting)
	// Level in which the enemies are visible. Default 80.
	float VisibleLevel = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AEnemyAIController> HDAIController;


	UPROPERTY(EditDefaultsOnly)
	float ExtraJumpHeight = 50.f;

private:
	void UpdateVisibility();

	UPROPERTY()
	TMap<AActor*, int32> ReservedAttackTokens;

	int32 TokensUsedInCurrentAttack;
public:

	/* Public Setters and Getters */
	
	void SetLevel(int32 InLevel) { Level = InLevel; }
	
	UFUNCTION(BlueprintCallable)
	void SetInCombat(bool InCombat);

	/** Combat Interface **/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse, bool bRagdoll = true) override;
	/**/

	/* Enemy Interface */

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual APatrolRoute* GetPatrolRoute_Implementation() const override;
	virtual float SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed) override;
	virtual void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
	virtual void JumpToDestination_Implementation(FVector Destination) override;
	virtual bool AttackStart_Implementation(AActor* AttackTarget, int32 TokensNeeded) override;
	virtual void AttackEnd_Implementation(AActor* AttackTarget) override;
	virtual void StoreAttackTokens(AActor* AttackTarget, int32 Amount);
	/**/

};

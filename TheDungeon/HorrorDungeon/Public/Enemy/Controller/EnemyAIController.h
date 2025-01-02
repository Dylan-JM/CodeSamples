
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GlobalEnums.h"
#include "EnemyAIController.generated.h"

class ABaseEnemy;
class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
struct FAIStimulus;

UCLASS()
class HORRORDUNGEON_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

	/* AI Perception*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	UFUNCTION(BlueprintCallable)
	void SetAIState(int32 StateValue, FVector Location = FVector(0,0,0), AActor* TargetActor = nullptr);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY()
	TArray<AActor*> KnownSeenActors;
	/* End AI */
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintNativeEvent)
	void HandleLostSight(AActor* Actor);
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
private:

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& Actors);
	bool CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& OutStimulus);
	
	void CheckIfForgottenSeenActor();
	FTimerHandle CheckForgottenTimer;

	void SeekAttackTarget();
	FTimerHandle SeekTargetHandle;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	float SeekTimeAfterLosingSight = 3.f;
	
	uint8 GetAIState(); 

	UPROPERTY(EditDefaultsOnly, Category="AI")
	int32 DefaultState = 0;
	
	TObjectPtr<ABaseEnemy> ControlledEnemy;

	void HandleSensedSight(AActor* Actor);
	void HandleSensedSound(FVector Location);
	void HandleSensedDamage(AActor* Actor);
	void HandleForgotActor(AActor* Actor);
public:

};

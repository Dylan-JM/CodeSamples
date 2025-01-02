
#include "Enemy/Controller/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "PlayerCharacter/PlayerCharacter.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard)
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviourTreeComponent");
	check(BehaviorTreeComponent);

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception Component");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	PerceptionComponent->ConfigureSense(*SightConfig);
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
	PerceptionComponent->ConfigureSense(*HearingConfig);
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("Damage Config");
	PerceptionComponent->ConfigureSense(*DamageConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::PerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledEnemy = Cast<ABaseEnemy>(InPawn);

	float AttackRadius;
	float DefendRadius;
	IEnemyInterface::Execute_GetIdealRange(ControlledEnemy, AttackRadius, DefendRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRadius", AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat("DefendRadius", DefendRadius);
	
	GetWorldTimerManager().SetTimer(CheckForgottenTimer, this, &AEnemyAIController::CheckIfForgottenSeenActor, 1.f, true);
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	CheckForgottenTimer.Invalidate();
}

void AEnemyAIController::PerceptionUpdated(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		FAIStimulus AIStimulus;
		if (CanSenseActor(Actor, EAISense::EAISense_Sight, AIStimulus))
		{
			HandleSensedSight(Actor);
		}
		else if (!CanSenseActor(Actor, EAISense::EAISense_Sight, AIStimulus))
		{
			HandleLostSight(Actor);
		}
		if (CanSenseActor(Actor, EAISense::EAISense_Hearing, AIStimulus))
		{
			HandleSensedSound(AIStimulus.StimulusLocation);
		}
		if (CanSenseActor(Actor, EAISense::EAISense_Damage, AIStimulus))
		{
			HandleSensedDamage(Actor);
		}
	}
}

bool AEnemyAIController::CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& OutStimulus)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	PerceptionComponent->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);
	for (FAIStimulus AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(GetPawn(), AIStimulus);
		switch (AISense)
		{
		case EAISense::EAISense_None:
			return false;
			
		case EAISense::EAISense_Sight:
			if (Sense == SightConfig->GetSenseImplementation())
			{
				OutStimulus = AIStimulus;
				return AIStimulus.WasSuccessfullySensed();
			}
			break;
		
		case EAISense::EAISense_Hearing:
			if (Sense == HearingConfig->GetSenseImplementation())
			{
				OutStimulus = AIStimulus;
				return AIStimulus.WasSuccessfullySensed();
			}
			break;

		case EAISense::EAISense_Damage:
			if (Sense == DamageConfig->GetSenseImplementation())
			{
				OutStimulus = AIStimulus;
				return AIStimulus.WasSuccessfullySensed();
			}
			break;
		}
	}
	return false;
}

void AEnemyAIController::HandleSensedSight(AActor* Actor)
{
	if (!Actor->IsA(ABaseCharacter::StaticClass())) return;
	if (ICombatInterface::Execute_OnSameTeam(ControlledEnemy, Cast<ABaseCharacter>(Actor))) return;
	KnownSeenActors.AddUnique(Actor);
	switch (GetAIState())
	{
	case 0:
			SetAIState(1, FVector(), Actor);
		break;
	case 1:
		if (Actor == AttackTarget)
		{
			SeekTargetHandle.Invalidate();
		}
		break;
	case 3:
		SetAIState(1, FVector(), Actor);
		break;
	case 5:
		SetAIState(1, FVector(), Actor);
		break;
	}
}

void AEnemyAIController::HandleSensedSound(FVector Location)
{
	switch (GetAIState())
	{
	case 0:
		SetAIState(3, Location);
		break;
	case 3:
		SetAIState(3, Location);
		break;
	case 5:
		SetAIState(3, Location);
		break;
	}
}

void AEnemyAIController::HandleSensedDamage(AActor* Actor)
{
	if (ICombatInterface::Execute_OnSameTeam(ControlledEnemy, Actor)) return;
	switch (GetAIState())
	{
	case 0:
		SetAIState(1, FVector(), Actor);
		break;
	case 3:
		SetAIState(1, FVector(), Actor);
		break;
	case 5:
		SetAIState(1, FVector(), Actor);
		break;
	}
}

void AEnemyAIController::HandleForgotActor(AActor* Actor)
{
	GetWorldTimerManager().PauseTimer(CheckForgottenTimer);
	KnownSeenActors.Remove(Actor);
	if (Actor == AttackTarget)
	{
		SetAIState(0);
	}
}

void AEnemyAIController::HandleLostSight_Implementation(AActor* Actor)
{
	GetWorldTimerManager().UnPauseTimer(CheckForgottenTimer);
	if (Actor == AttackTarget)
	{
		if (GetAIState() == 1 || GetAIState() == 2 || GetAIState() == 3)
		{
			SeekTargetHandle.Invalidate();
			SetAIState(5, AttackTarget->GetActorLocation());
			GetWorldTimerManager().SetTimer(SeekTargetHandle, this, &AEnemyAIController::SeekAttackTarget, SeekTimeAfterLosingSight, false);
		} 
	}
}

void AEnemyAIController::CheckIfForgottenSeenActor()
{
	TArray<AActor*> OutActors;
	PerceptionComponent->GetKnownPerceivedActors(SightConfig->GetSenseImplementation(), OutActors);
	if (!OutActors.Num() == KnownSeenActors.Num())
	{
		for (AActor* Actor : KnownSeenActors)
		{
			if (OutActors.Find(Actor) == -1)
			{
				HandleForgotActor(Actor);
			}
		}
	}
}

void AEnemyAIController::SeekAttackTarget()
{
	// Set state as seeking
	SeekTargetHandle.Invalidate();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	SetAIState(DefaultState);
}

void AEnemyAIController::SetAIState(int32 StateValue, FVector Location, AActor* TargetActor)
{
	if (StateValue == 1)
	{
		if (TargetActor->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(TargetActor))
		{
			SetAIState(0);
		}
		else
		{
			AttackTarget = TargetActor;
			GetBlackboardComponent()->SetValueAsObject("TargetToFollow", TargetActor);
		}
	}
	if (StateValue == 3)
	{
		GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Location);
	}
	if (StateValue == 5)
	{
		GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Location);
	}
	GetBlackboardComponent()->SetValueAsEnum("State", StateValue);
}

uint8 AEnemyAIController::GetAIState()
{
	return GetBlackboardComponent()->GetValueAsEnum("State");
}




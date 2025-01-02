
#include "Enemy/BaseEnemy.h"
#include "HDGameplayTags.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Abilities/HDAttributeSet.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HDAbilitySystemComponent.h"
#include "Components/LightingComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "HUD/Widget/MainHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter/BaseCharacter.h"
#include "Weapons/BaseWeapon.h"


ABaseEnemy::ABaseEnemy(const class FObjectInitializer& ObjectInitializer) : ABaseCharacter(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UHDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSetBase = CreateDefaultSubobject<UHDAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 450.f;
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!StopUpdateVisibility)
	{
		UpdateVisibility();
	}
}

void ABaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	HDAIController = Cast<AEnemyAIController>(NewController);
	if (BehaviorTree)
	{
		HDAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		HDAIController->RunBehaviorTree(BehaviorTree);
	}
	HDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	HDAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

FVector ABaseEnemy::CalculateFutureActorLocation(AActor* Actor, float Time)
{
	if (Actor->GetVelocity() == FVector(0,0,0))
	{
		return Actor->GetActorLocation();
	}
	return GetActorLocation() + (Actor->GetVelocity() * FVector(1.f,1.f,0.f)) * Time;
	
}

void ABaseEnemy::SetInCombat(bool InCombat)
{
	if (bIsAiming == InCombat) return;
	bIsAiming = InCombat;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UHDAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (UMainHUD* MainHUDWidget = Cast<UMainHUD>(HealthBar->GetUserWidgetObject()))
	{
		MainHUDWidget->SetWidgetController(this);
	}
	
	if (const UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HDAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HDAS->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FHDGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ABaseEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(HDAS->GetHealth());
		OnMaxHealthChanged.Broadcast(HDAS->GetMaxHealth());
	}

	SpawnWeapons();

	
}

void ABaseEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UHDAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FHDGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ABaseEnemy::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void ABaseEnemy::InitializeDefaultAttributes() const
{
	UHDAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ABaseEnemy::Die(const FVector& DeathImpulse, bool bRagdoll)
{
	if (HDAIController)
	{
		HDAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	TArray<AActor*> OutKey;
	ReservedAttackTokens.GetKeys(OutKey);
	for (AActor* Actor : OutKey)
	{
		ICombatInterface::Execute_ReturnAttackToken(Actor, ReservedAttackTokens.FindRef(Actor));
	}
	if (EquippedWeapon1)
	{
		EquippedWeapon1->Destroy();
	}
	if (EquippedOffhandItem)
	{
		EquippedOffhandItem->Destroy();
	}
	if (bHasItem)
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GOLD);
	}
	Super::Die(DeathImpulse);
}

void ABaseEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ABaseEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

APatrolRoute* ABaseEnemy::GetPatrolRoute_Implementation() const
{
	return PatrolRoute;
}

float ABaseEnemy::SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed)
{
	switch (MovementSpeed)
	{
	case EMovementSpeed::EIT_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		break;
	case EMovementSpeed::EIT_Walking:
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
		break;
	case EMovementSpeed::EIT_Jogging:
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		break;
	case EMovementSpeed::EIT_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = 450.f;
		break;
	}
	return GetCharacterMovement()->MaxWalkSpeed;
	
}

void ABaseEnemy::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 150.f;
	DefendRadius = 350.f;
}

void ABaseEnemy::JumpToDestination_Implementation(FVector Destination)
{
	FVector OutVelocity;
	FVector EndPosition = FVector(Destination.X, Destination.Y, Destination.Z + ExtraJumpHeight);
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutVelocity, GetActorLocation(), EndPosition);
	LaunchCharacter(OutVelocity, true, true);
}

bool ABaseEnemy::AttackStart_Implementation(AActor* AttackTarget, int32 TokensNeeded)
{
	bIsAttacking = true;
	if (AttackTarget && AttackTarget->Implements<UCombatInterface>() && ICombatInterface::Execute_ReserveAttackToken(AttackTarget, TokensNeeded))
	{
		StoreAttackTokens(AttackTarget, TokensNeeded);
		TokensUsedInCurrentAttack = TokensNeeded;
		return true;
	}
	return false;
}

void ABaseEnemy::AttackEnd_Implementation(AActor* AttackTarget)
{
	bIsAttacking = false;
	if (AttackTarget && AttackTarget->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_ReturnAttackToken(AttackTarget, TokensUsedInCurrentAttack);
	}
	StoreAttackTokens(AttackTarget, -TokensUsedInCurrentAttack);
	AttackFinishedDelegate.Broadcast();
}

void ABaseEnemy::StoreAttackTokens(AActor* AttackTarget, int32 Amount)
{
	int32 ATokens = Amount + ReservedAttackTokens.FindRef(AttackTarget);
	bool ItemFound = (ReservedAttackTokens.Find(AttackTarget) != nullptr);
	int32 TokensToAdd = UKismetMathLibrary::SelectInt(ATokens,Amount, ItemFound);
	ReservedAttackTokens.Add(AttackTarget, TokensToAdd);
}

void ABaseEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::HitReactTagChanged(CallbackTag, NewCount);
	if (HDAIController && HDAIController->GetBlackboardComponent())
	{
		HDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}


void ABaseEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (HDAIController && HDAIController->GetBlackboardComponent())
	{
		HDAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void ABaseEnemy::UpdateVisibility()
{
	float LightingAmount = LightingComponent->GetLightingAmount();
	if (LightingAmount > VisibleLevel && GetMesh()->IsVisible())
	{
		return;
	}
	if (LightingAmount < VisibleLevel)
	{
		GetMesh()->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
	}
}

int32 ABaseEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}
// Copyright DM


#include "PlayerCharacter/HealthCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Abilities/HDAttributeSet.h"
#include "Components/HDAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter/PlayerCharacter.h"

AHealthCharacter::AHealthCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UHDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSetBase = CreateDefaultSubobject<UHDAttributeSet>("AttributeSet");
}

void AHealthCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHealthCharacter, bCanRevive);
}

void AHealthCharacter::Die(const FVector& DeathImpulse, bool bRagdoll)
{
	bCanRevive = false;
}

void AHealthCharacter::PlayerAdded(APlayerCharacter* PlayerCharacter)
{
	if (!HasAuthority()) return;
	PlayerCharacter->GetOnDeathDelegate().AddDynamic(this, &AHealthCharacter::PlayerDied);
	PlayerCharacter->OnPlayerRevived.AddDynamic(this, &AHealthCharacter::PlayerRevived);
}

void AHealthCharacter::PlayerDied(AActor* DeadPlayer)
{
	if (AbilitySystemComponent)
	{
		if (HealthDepletion)
		{
			FGameplayEffectContextHandle GameplayEffectContext;
			GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(HealthDepletion.GetDefaultObject(), 1.f, GameplayEffectContext);
		}
	}
	BroadcastPlayerDied();
}

void AHealthCharacter::BroadcastPlayerDied_Implementation()
{
	PlayerDiedDelegate.Broadcast();
}

void AHealthCharacter::PlayerRevived()
{
	if (HealthDepletion)
	{
		const FGameplayEffectQuery GameplayEffectQuery;
		TArray<FActiveGameplayEffectHandle> ActiveEffects = GetAbilitySystemComponent()->GetActiveEffects(GameplayEffectQuery);
		GetAbilitySystemComponent()->RemoveActiveGameplayEffect(ActiveEffects[0]);
	}
}

void AHealthCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	
	if (const UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HDAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
				if (Data.NewValue <= 0.f)
				{
					Die(FVector());
					TArray<AActor*> PlayersInWorld;
					UGameplayStatics::GetAllActorsOfClass(this, APlayerCharacter::StaticClass(), PlayersInWorld);
					for (AActor* Player : PlayersInWorld)
					{
						APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player);
						if (PlayerCharacter && PlayerCharacter->bDeadOnGround)
						{
							PlayerCharacter->EmptyInventory();
							PlayerCharacter->ClientSaveProgress();
						}
					}
				}
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HDAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(HDAS->GetHealth());
		OnMaxHealthChanged.Broadcast(HDAS->GetMaxHealth());
	}
}

void AHealthCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UHDAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		UHDAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, 1.f, AbilitySystemComponent);
	}
	
	OnAscRegistered.Broadcast(AbilitySystemComponent);

}

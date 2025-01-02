// Copyright DM


#include "Weapons/MeleeWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter/BaseCharacter.h"


AMeleeWeapon::AMeleeWeapon()
{
	AbilityWeaponBox = CreateDefaultSubobject<UBoxComponent>("AbilityWeaponBox");
	AbilityWeaponBox->SetupAttachment(GetRootComponent());

	AbilityWeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityWeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	AbilityWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnAbilityBoxOverlap);
}

void AMeleeWeapon::OnAbilityBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || AbilityActorsToIgnore.Contains(OtherActor) || !GetOwner()->HasAuthority() || !OtherActor->IsA(ABaseCharacter::StaticClass())) return;
	if (ICombatInterface::Execute_OnSameTeam(GetOwner(), OtherActor)) return;
		
	AbilityOverlappingActors.Add(OtherActor);
	for (AActor* OverlappingActor : AbilityOverlappingActors)
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Yellow, OverlappingActor->GetName());
		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				FGameplayCueParameters GameplayCueParameters;
				GameplayCueParameters.Location = OtherActor->GetActorLocation();
				TargetASC->ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

				if (DebuffParams.TargetAbilitySystemComponent->IsValidLowLevel())
				{
					UHDAbilitySystemLibrary::ApplyDamageEffect(DebuffParams);
				}
			}
		}
		AbilityActorsToIgnore.AddUnique(OverlappingActor);
	}
	AbilityOverlappingActors.Empty();
}

void AMeleeWeapon::AbilityHandleCollision(bool bEnable)
{
	if (bEnable)
	{
		AbilityWeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		AbilityWeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AbilityWeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		AbilityWeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AbilityActorsToIgnore.Empty();
	}
}

// Copyright DM


#include "Actor/HDFlareProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Abilities/HDAttributeSet.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "Weapons/BaseWeapon.h"

void AHDFlareProjectile::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AHDFlareProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
			{
				UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(PlayerCharacter->GetAttributeSet());
				DamageEffectParams.DebuffDamage = (HDAS->GetIntelligence() * PlayerCharacter->EquippedWeapon1->DamageMultiplier);
				DamageEffectParams.BaseDamage = 0.1f;
			}

			UHDAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
	
}

void AHDFlareProjectile::OnHit()
{
	
}

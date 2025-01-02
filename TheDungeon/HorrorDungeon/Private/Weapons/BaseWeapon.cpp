

#include "Weapons/BaseWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Weapon Box"));
	WeaponCapsule->SetupAttachment(RootComponent);
	WeaponCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::OnOwnerDead(const FVector& DeathImpulse)
{
	Super::OnOwnerDead(DeathImpulse);
	WeaponCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ABaseWeapon::HandleCollision(bool bEnable)
{
	if (bEnable)
	{
		GetWeaponCapsule()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		GetWeaponCapsule()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		GetWeaponCapsule()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetWeaponCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EmptySameActorArray();
	}
}

void ABaseWeapon::EmptySameActorArray()
{
	ActorsToIgnore.Empty();
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnCapsuleOverlap);
}

void ABaseWeapon::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorsToIgnore.Contains(OtherActor) || !GetOwner()->HasAuthority() || !OtherActor->IsA(ABaseCharacter::StaticClass())) return;
	if (ICombatInterface::Execute_OnSameTeam(GetOwner(), OtherActor)) return;
	
	OverlappingActors.Add(OtherActor);
	for (AActor* OverlappingActor : OverlappingActors)
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
				
				TArray<FHitResult> HitResults;
				FComponentQueryParams Params;
				Params.AddIgnoredActor(this);
				GetWorld()->ComponentSweepMulti(HitResults, WeaponCapsule, WeaponCapsule->GetComponentLocation() + FVector(-.1f), WeaponCapsule->GetComponentLocation() + FVector(0.1f), WeaponCapsule->GetComponentRotation(), Params);
				/*for (FHitResult HitResult : HitResults)
				{
					DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 12, FColor::Yellow, false, 5.f);
				}*/
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BloodEffect, HitResults[0].ImpactPoint);
				UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResults[0].ImpactPoint);
			}
		}
		ActorsToIgnore.AddUnique(OverlappingActor);
	}
	OverlappingActors.Empty();
}

/*if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
{
	if (ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OtherActor))
	{
		if (BaseEnemy->AttackType == EAttackType::EAT_Melee)
		{
			if (!PlayerCharacter->MeleeAttackers.Contains(OtherActor))
			{
				PlayerCharacter->MeleeAttackers.Pop();
				PlayerCharacter->MeleeAttackers.AddUnique(Cast<APawn>(OtherActor));
			}
		}
		else if (BaseEnemy->AttackType == EAttackType::EAT_Ranged)
		{
			if (!PlayerCharacter->RangedAttackers.Contains(OtherActor))
			{
				PlayerCharacter->RangedAttackers.Pop();
				PlayerCharacter->RangedAttackers.AddUnique(Cast<APawn>(OtherActor));
			}
		}
	}

}*/

// Copyright DM


#include "Abilities/GA/HDBeamSpell.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "Weapons/BaseWeapon.h"

void UHDBeamSpell::StoreDataInfo()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		FVector TempHitLocation = PlayerCharacter->GetTraceResult(TraceDistance).ImpactPoint;
		AActor* StoredTargetActor = nullptr;
		StoredTargetActor = PlayerCharacter->GetTraceResult(TraceDistance).GetActor();
		ServerStoreDataInfo(TempHitLocation, StoredTargetActor);
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(StoredTargetActor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UHDBeamSpell::PrimaryTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UHDBeamSpell::PrimaryTargetDied);
			}
		}
	}
}

void UHDBeamSpell::ServerStoreDataInfo_Implementation(FVector TempHitLocation, AActor* TempHitActor)
{
	MulticastStoreDataInfo(TempHitLocation, TempHitActor);
}

void UHDBeamSpell::MulticastStoreDataInfo_Implementation(FVector TempHitLocation, AActor* TempHitActor)
{
	HitLocation = TempHitLocation;
	HitActor = TempHitActor;
}

void UHDBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	if (!HitActor->IsValidLowLevel()) return;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(HitActor);
	
	TArray<AActor*> OverlappingActors;
	UHDAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlappingActors, ActorsToIgnore, 850.f, HitActor->GetActorLocation());
	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	
	UHDAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, HitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UHDBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UHDBeamSpell::AdditionalTargetDied);
			}
		}
	}
}

void UHDBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (ABaseWeapon* EquippedWeapon = ICombatInterface::Execute_GetEquippedWeapon1(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = EquippedWeapon->GetItemMesh()->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter, SocketLocation, BeamTargetLocation, 10.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
			if (HitResult.bBlockingHit)
			{
				ServerStoreDataInfo(HitResult.ImpactPoint, HitResult.GetActor());
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UHDBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UHDBeamSpell::PrimaryTargetDied);
		}
	}
}

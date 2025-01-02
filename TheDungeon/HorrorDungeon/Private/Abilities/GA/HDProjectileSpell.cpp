// Copyright DM


#include "Abilities/GA/HDProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/HDProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Weapons/Bow.h"
#include "Weapons/MageWeapon.h"



void UHDProjectileSpell::SpawnProjectile()
{
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		FRotator TargetRotation;
		FTransform OutTransform;
		FVector ToTarget;
		const UStaticMeshSocket* TipSocket = PlayerCharacter->EquippedWeapon1->GetItemMesh()->GetSocketByName(FName("TipSocket"));
		if (TipSocket)
		{
			TipSocket->GetSocketTransform(OutTransform, PlayerCharacter->EquippedWeapon1->GetItemMesh());
		}

		if (ABow* Bow = Cast<ABow>(PlayerCharacter->EquippedWeapon1))
		{
			const USkeletalMeshSocket* SkeletalTipSocket = Bow->BowMeshComponent->GetSocketByName(FName("TipSocket"));
			OutTransform = SkeletalTipSocket->GetSocketTransform(Bow->BowMeshComponent);
		}
		
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());

		HitTargetLoc = PlayerCharacter->GetTraceResult(TraceDistance).ImpactPoint;
		ToTarget = HitTargetLoc - OutTransform.GetLocation();
		TargetRotation = ToTarget.Rotation();
		//TargetRotation = PlayerCharacter->GetActorRotation();
		
		if (PlayerCharacter->IsLocallyControlled())
		{
			ServerSpawnProjectile(OutTransform, TargetRotation);
		}
	}
	if (ABaseEnemy* RangedEnemy = Cast<ABaseEnemy>(GetAvatarActorFromActorInfo()))
	{
		if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetAvatarActorFromActorInfo()->GetInstigatorController()))
		{
			UObject* TargetObject = AIController->GetBlackboardComponent()->GetValueAsObject("TargetToFollow");
			AActor* TargetActor = Cast<AActor>(TargetObject);
			FRotator TargetRotation;
			FTransform OutTransform;
			FVector ToTarget;
			const UStaticMeshSocket* TipSocket = RangedEnemy->EquippedWeapon1->GetItemMesh()->GetSocketByName(FName("TipSocket"));
			TipSocket->GetSocketTransform(OutTransform, RangedEnemy->EquippedWeapon1->GetItemMesh());
			if (TargetActor)
			{
				ToTarget = TargetActor->GetActorLocation() - OutTransform.GetLocation();
				TargetRotation = ToTarget.Rotation();
				ServerSpawnProjectile(OutTransform, TargetRotation);
			}
		}
	}
}

void UHDProjectileSpell::ServerSpawnProjectile_Implementation(FTransform OutTransform, FRotator TargetRotation)
{
	TObjectPtr<AHDProjectile> SpawnedProjectile = nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	
	SpawnedProjectile = GetWorld()->SpawnActor<AHDProjectile>(ProjectileClass, OutTransform.GetLocation(), TargetRotation, SpawnParams);
	
	SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
}

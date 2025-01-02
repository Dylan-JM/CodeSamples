// Copyright DM


#include "Abilities/GA/HDEnemyTeleport.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"

FVector UHDEnemyTeleport::GetTeleportLocation() const
{
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetAvatarActorFromActorInfo());
	AEnemyAIController* Controller = Cast<AEnemyAIController>(Enemy->GetController());

	return Controller->GetBlackboardComponent()->GetValueAsVector("PointOfInterest");
}

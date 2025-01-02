// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Services/BTService_IsPlayerInMeleeRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "PlayerCharacter/PlayerCharacter.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Controller->GetPawn());

	// get player character
	if (UBlackboardComponent* bc = OwnerComp.GetBlackboardComponent())
	{
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(bc->GetValueAsObject("TargetPlayer"));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->GetDistanceTo(TargetPlayer) <= MeleeRange);
	}
	
}

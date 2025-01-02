// Copyright DM


#include "Actor/EnemySpawnPoint.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/Controller/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/PlayerCharacter.h"

void AEnemySpawnPoint::SpawnEnemy()
{
	if (!HasAuthority()) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseEnemy* Enemy = GetWorld()->SpawnActorDeferred<ABaseEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->CharacterClass = CharacterClass;
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}

void AEnemySpawnPoint::SpawnExtractionEnemy()
{
	if (!HasAuthority()) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseEnemy* Enemy = GetWorld()->SpawnActorDeferred<ABaseEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->CharacterClass = CharacterClass;
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerCharacter::StaticClass(), Players);
	
	
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(Enemy->GetController());
	if (EnemyAIController)
	{
		EnemyAIController->AttackTarget = Players[rand() % (Players.Num())];
	}
}

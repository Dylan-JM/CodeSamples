// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaveInterface.h"
#include "EnemySpawnVolume.generated.h"

class AEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class HORRORDUNGEON_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnVolume();

	/* Save Interface */
	// IF WANT TO SAVE THE VOLUMES ADD INHERIT FROM INTERFACE AND ADD FUNCTION BACK IN and add SaveGame to bReached UPROPERTY
	//virtual void LoadActor_Implementation() override;
	/* End Save Interface */

	UPROPERTY(BlueprintReadOnly)
	bool bReached = false;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawnPoint*> SpawnPoints;
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
};

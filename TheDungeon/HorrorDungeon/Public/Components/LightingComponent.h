// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightingComponent.generated.h"


class APointLight;
class ABasePlayerController;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORDUNGEON_API ULightingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULightingComponent();

	UFUNCTION(BlueprintCallable)
	float GetLightingAmount();

	TObjectPtr<APointLight> PointLight;
	
protected:
	virtual void BeginPlay() override;


	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
	UPROPERTY()
	TObjectPtr<ABasePlayerController> Controller;

		
};

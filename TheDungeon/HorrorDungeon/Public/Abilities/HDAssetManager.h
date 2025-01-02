// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UHDAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UHDAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};

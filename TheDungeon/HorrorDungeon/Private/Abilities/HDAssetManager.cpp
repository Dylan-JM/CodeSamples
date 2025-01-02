// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HDAssetManager.h"
#include "HDGameplayTags.h"
#include "AbilitySystemGlobals.h"

UHDAssetManager& UHDAssetManager::Get()
{
	check(GEngine);
	UHDAssetManager* HDAssetManager = Cast<UHDAssetManager>(GEngine->AssetManager);
	return *HDAssetManager;
}

void UHDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FHDGameplayTags::InitializeNativeGameplayTags();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
}

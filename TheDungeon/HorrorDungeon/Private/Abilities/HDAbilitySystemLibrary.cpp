// Copyright DM


#include "Abilities/HDAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HDAbilityTypes.h"
#include "HDGameplayTags.h"
#include "HorrorDungeonGameMode.h"
#include "Game/LoadScreenSaveGame.h"
#include "HUD/BaseHUD.h"
#include "HUD/WidgetController/HDWidgetController.h"
#include "Interfaces/CombatInterface.h"
#include "PlayerState/BasePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

bool UHDAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ABaseHUD*& OutBaseHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutBaseHUD = Cast<ABaseHUD>(PC->GetHUD());
		if (OutBaseHUD)
		{
			ABasePlayerState* PS = PC->GetPlayerState<ABasePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSetBase();
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UHDAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetOverlayWidgetController(WCParams);
	}
	
	return nullptr;
}

UAttributeMenuWidgetController* UHDAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UHDAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

UInventoryWidgetController* UHDAbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetInventoryWidgetController(WCParams);
	}
	return nullptr;
}

void UHDAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UHDAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FHDGameplayTags& GameplayTags = FHDGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UHDAbilitySystemLibrary::InitializeDefaultAttributesClientFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, float InStrength, float InInt, float InRes, float InVigor)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FHDGameplayTags& GameplayTags = FHDGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, InStrength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, InInt);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, InRes);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, InVigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UHDAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UHDAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

void UHDAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UHDAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
	
}

void UHDAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
	
}

void UHDAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

UCharacterClassInfo* UHDAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AHorrorDungeonGameMode* HDGameMode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (HDGameMode == nullptr) return nullptr;

	return HDGameMode->CharacterClassInfo;
}

UAbilityInfo* UHDAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AHorrorDungeonGameMode* HDGameMode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (HDGameMode == nullptr) return nullptr;

	return HDGameMode->AbilityInfo;
}

bool UHDAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->IsBlockedHit();
	}
	return false;
}

bool UHDAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->IsCriticalHit();
	}
	return false;
}

bool UHDAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UHDAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UHDAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UHDAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UHDAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (HDEffectContext->GetDamageType().IsValid())
		{
			return *HDEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UHDAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UHDAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UHDAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->IsRadialDamage();
	}
	return false;
}

float UHDAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UHDAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UHDAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

bool UHDAbilitySystemLibrary::IsBlockable(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHDGameplayEffectContext* HDEffectContext = static_cast<const FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HDEffectContext->IsBlockable();
	}
	return false;
}

void UHDAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UHDAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UHDAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInSuccessfulDebuff)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UHDAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float bInDamage)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetDebuffDamage(bInDamage);
	}
}

void UHDAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float bInDuration)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetDebuffDuration(bInDuration);
	}
}

void UHDAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float bInFrequency)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetDebuffFrequency(bInFrequency);
	}
}

void UHDAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		HDEffectContext->SetDamageType(DamageType);
	}
}

void UHDAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UHDAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetKnockbackForce(InForce);
	}
}

void UHDAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UHDAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UHDAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UHDAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UHDAbilitySystemLibrary::SetIsBlockable(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsBlockable)
{
	if (FHDGameplayEffectContext* HDEffectContext = static_cast<FHDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HDEffectContext->SetIsBlockable(bInIsBlockable);
	}
}

void UHDAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlappingActors, const TArray<AActor*> ActorsToIgnore, float Radius,
                                                         const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UHDAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UHDAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle UHDAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FHDGameplayTags& GameplayTags = FHDGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	SetIsBlockable(EffectContextHandle, DamageEffectParams.bBlockable);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
	
}

TArray<FRotator> UHDAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2, FVector::UpVector);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UHDAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2, FVector::UpVector);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

TArray<FVector> UHDAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Start, const FVector& End, float Spread,
	int32 NumVectors)
{
	TArray<FVector> Vectors;
	Vectors.Reserve(NumVectors);

	FVector Diff = (End - Start) / (float) NumVectors;
	for (int i = 0; i < NumVectors; ++i)
	{
		Vectors.Add(Start + (i * Diff));
	}

	return Vectors;
}



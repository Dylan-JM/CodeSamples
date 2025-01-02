// Copyright DM


#include "HUD/WidgetController/HDWidgetController.h"
#include "Abilities/HDAttributeSet.h"
#include "Abilities/Data/AbilityInfo.h"
#include "Components/HDAbilitySystemComponent.h"
#include "PlayerController/BasePlayerController.h"
#include "PlayerState/BasePlayerState.h"

void UHDWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UHDWidgetController::BroadcastInitialValues()
{
	
}

void UHDWidgetController::BindCallbacksToDependencies()
{
}

void UHDWidgetController::BroadcastAbilityInfo()
{
	if (!GetHDASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FHDAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(HDAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = HDAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = HDAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetHDASC()->ForEachAbility(BroadcastDelegate);
}

ABasePlayerController* UHDWidgetController::GetBasePC()
{
	if (BasePlayerController == nullptr)
	{
		BasePlayerController = Cast<ABasePlayerController>(PlayerController);
	}
	return BasePlayerController;
}

ABasePlayerState* UHDWidgetController::GetBasePS()
{
	if (BasePlayerState == nullptr)
	{
		BasePlayerState = Cast<ABasePlayerState>(PlayerState);
	}
	return BasePlayerState;
}

UHDAbilitySystemComponent* UHDWidgetController::GetHDASC()
{
	if (HDAbilitySystemComponent == nullptr)
	{
		HDAbilitySystemComponent = Cast<UHDAbilitySystemComponent>(AbilitySystemComponent);
	}
	return HDAbilitySystemComponent;
}

UHDAttributeSet* UHDWidgetController::GetHDAttributeSet()
{
	if (HDAttributeSet == nullptr)
	{
		HDAttributeSet = Cast<UHDAttributeSet>(AttributeSet);
	}
	return HDAttributeSet;
}

// Copyright DM


#include "HUD/WidgetController/AttributeMenuWidgetController.h"
#include "Abilities/HDAttributeSet.h"
#include "Abilities/Data/AttributeInfo.h"
#include "Components/HDAbilitySystemComponent.h"
#include "PlayerState/BasePlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair: GetHDAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
	[this, Pair](const FOnAttributeChangeData& Data)
	{
		BroadCastAttributeInfo(Pair.Key, Pair.Value());
	});
	}
	
	GetBasePS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UHDAttributeSet* AS = Cast<UHDAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair: AS->TagsToAttributes)
	{
		BroadCastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	AttributePointsChangedDelegate.Broadcast(GetBasePS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	UHDAbilitySystemComponent* HDASC = CastChecked<UHDAbilitySystemComponent>(AbilitySystemComponent);
	HDASC->UpdateAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadCastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FHDAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

// Copyright Benedikt Schnatterbeck


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

/**
 * @brief Broadcasts the initial values of the attributes to the UI.
 * 
 * This method requires a valid AttributeSet and should only
 * be called the widget controller has been set, in this case
 * UAuraUserWidget::SetWidgetController
 */
void UOverlayWidgetController::BroadcastInitialValues()
{
	checkf(AttributeSet, TEXT("AttributeSet is not set in %hs This should never happen."), __FUNCTION__);

	const auto AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	checkf(AuraAttributeSet,
	       TEXT("Could not cast to type UAuraAttributeSet in %hs -> BroadcastInitialValues. This should never happen"),
	       __FUNCTION__);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
	checkf(AttributeSet, TEXT("AttributeSet is not set in %hs This should never happen."), __FUNCTION__);

	const auto AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	checkf(AuraAttributeSet, TEXT("Could not cast to type UAuraAttributeSet in %hs This should never happen"),
	       __FUNCTION__);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnHealthChanged.Broadcast(Data.NewValue);
	                      });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxHealthChanged.Broadcast(Data.NewValue);
	                      });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnManaChanged.Broadcast(Data.NewValue);
	                      });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxManaChanged.Broadcast(Data.NewValue);
	                      });

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}

// Copyright Benedikt Schnatterbeck


#include "UI/WidgetController/OverlayWidgetController.h"
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
	                      .AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

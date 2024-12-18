// Copyright Benedikt Schnatterbeck


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   const bool bLogNotFound) const
{
	const FAuraInputAction* AuraInputAction = AbilityInputActions.FindByPredicate(
		[InputTag](const FAuraInputAction& Action)
		{
			if (!Action.InputAction) return false;
			return Action.InputTag.MatchesTag(InputTag);
		});

	if (const auto Action = AuraInputAction->InputAction) return Action;

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]"),
		       *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

// Copyright Benedikt Schnatterbeck


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (const auto Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag) { return Info; }
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("[%s]: AttributeInfo for Tag [%s] not found"),
		       *GetNameSafe(this), *AttributeTag.ToString()
		);
	}

	return FAuraAttributeInfo();
}

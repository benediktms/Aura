// Copyright Benedikt Schnatterbeck


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// this is no longer needed as of UE5.3
	// see: https://github.com/tranek/GASDocumentation?tab=readme-ov-file#491-initglobaldata
	// UAbilitySystemGlobals::Get().InitGlobalData();

	FAuraGameplayTags::InitializeNativeGameplayTags();
}

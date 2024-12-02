// Copyright Benedikt Schnatterbeck


#include "AuraAssetmanager.h"
#include "AuraGameplayTags.h"

UAuraAssetmanager& UAuraAssetmanager::Get()
{
	check(GEngine);

	UAuraAssetmanager* AuraAssetManager = Cast<UAuraAssetmanager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetmanager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
}

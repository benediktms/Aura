// Copyright Benedikt Schnatterbeck

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetmanager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetmanager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetmanager& Get();

protected:
	virtual void StartInitialLoading() override;
};

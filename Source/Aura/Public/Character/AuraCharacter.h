// Copyright Benedikt Schnatterbeck

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};

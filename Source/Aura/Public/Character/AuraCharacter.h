// Copyright Benedikt Schnatterbeck

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class AAuraPlayerState;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetActorLevel() override;
	/** end Combat Interface */

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

private:
	virtual void InitAbilityActorInfo() override;
};

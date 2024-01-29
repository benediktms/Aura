// Copyright Benedikt Schnatterbeck

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class UCharacterMovementComponent;
class AAuraEnemy;

UCLASS()
class AURA_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AAuraEnemy> EnemyCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
};

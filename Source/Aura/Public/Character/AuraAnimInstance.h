// Copyright Benedikt Schnatterbeck

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AuraAnimInstance.generated.h"

class UCharacterMovementComponent;
class AAuraCharacter;

UCLASS()
class AURA_API UAuraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AAuraCharacter> AuraCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
};

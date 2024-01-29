// Copyright Benedikt Schnatterbeck


#include "Character/AuraAnimInstance.h"
#include "Character/AuraCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAuraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (auto Owner = Cast<AAuraCharacter>(TryGetPawnOwner()))
	{
		AuraCharacter = Owner;
		CharacterMovement = Owner->GetCharacterMovement();
	}
}

void UAuraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AuraCharacter && CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

		if (GroundSpeed > 3.f)
		{
			bShouldMove = true;
		}
		else
		{
			bShouldMove = false;
		}
	}
}

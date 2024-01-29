// Copyright Benedikt Schnatterbeck


#include "Character/EnemyAnimInstance.h"
#include "Character/AuraEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (auto Owner = Cast<AAuraEnemy>(TryGetPawnOwner()))
	{
		EnemyCharacter = Owner;
		CharacterMovement = Owner->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (EnemyCharacter && CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
	}
}

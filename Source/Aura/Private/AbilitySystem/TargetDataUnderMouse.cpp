// Copyright Benedikt Schnatterbeck


#include "AbilitySystem/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Task = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Task;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	// Get the player controller by dereferencing the weak pointer
	const auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;

	PC->GetHitResultUnderCursor(
		ECC_Visibility,
		false,
		CursorHit
	);

	ValidData.Broadcast(CursorHit.Location);
}

// Copyright Benedikt Schnatterbeck


#include "AbilitySystem/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Task = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Task;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else // task is running on the Server
	{
		const auto SpecHandle = GetAbilitySpecHandle();
		const auto ActivationPredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent
			.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent
		                             .Get()->CallReplicatedTargetDataDelegatesIfSet(
			                             SpecHandle, ActivationPredictionKey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// Set prediction window
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// Get the player controller by dereferencing the weak pointer
	const auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;

	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	const auto Data = new FGameplayAbilityTargetData_SingleTargetHit();
	FGameplayAbilityTargetDataHandle DataHandle;

	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(), // no tag needed, so a temporary one is created
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

// Copyright Benedikt Schnatterbeck


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	PathSpline = CreateDefaultSubobject<USplineComponent>("PathSpline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld
	);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent) return AuraAbilitySystemComponent;

	AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));

	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (ThisActor == LastActor) return;
	if (LastActor != nullptr) LastActor->UnHighlightActor();
	if (ThisActor != nullptr) ThisActor->HighlightActor();
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = !!ThisActor;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UAuraAbilitySystemComponent* ASC = GetASC();
	if (ASC == nullptr) return;

	ASC->AbilityInputTagReleased(InputTag);

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) return;
	if (bTargeting || bShiftKeyDown) return;

	const APawn* ControlledPawn = GetPawn();

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		this,
		ControlledPawn->GetActorLocation(),
		CachedDestination
	);

	if (FollowTime <= ShortPressThreshold && ControlledPawn && NavPath)
	{
		PathSpline->ClearSplinePoints();
		for (const FVector& PointLoc : NavPath->PathPoints)
		{
			PathSpline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
		}

		CachedDestination = NavPath->PathPoints.Last();
		bAutoRunning = true;
	}

	bTargeting = false;
	FollowTime = 0.f;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UAuraAbilitySystemComponent* ASC = GetASC();

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (!ASC) return;
		ASC->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (!ASC) return;
		ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	const FVector LocationOnSpline = PathSpline->FindLocationClosestToWorldLocation(
		ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = PathSpline->FindDirectionClosestToWorldLocation(
		LocationOnSpline, ESplineCoordinateSpace::World);

	ControlledPawn->AddMovementInput(Direction);

	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}

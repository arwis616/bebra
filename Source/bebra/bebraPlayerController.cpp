// Copyright Epic Games, Inc. All Rights Reserved.

#include "bebraPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "bebraCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "bebra.h"
#include "DeckComponent.h"
#include "CardData.h"
#include "HudWidget.h"
#include "Camera/PlayerCameraManager.h"
#include "StatsComponent.h"
#include "Blueprint/UserWidget.h"
#include "CardEffect.h"
#include "Kismet/GameplayStatics.h"
#include "DeathScreenWidget.h"
#include "SettingsWidget.h"
#include "GameSettingsSave.h"

AbebraPlayerController::AbebraPlayerController()
{
	bIsTouch = false;
	bMoveToMouseCursor = false;

	// create the path following comp
	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("Path Following Component"));

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AbebraPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Setup mouse input events
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AbebraPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AbebraPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AbebraPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AbebraPlayerController::OnSetDestinationReleased);

			// Setup touch input events
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AbebraPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AbebraPlayerController::OnTouchTriggered);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AbebraPlayerController::OnTouchReleased);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AbebraPlayerController::OnTouchReleased);
			for (int32 i = 0; i < CardSlotActions.Num(); i++)
			{
				if (CardSlotActions[i])
				{
					EnhancedInputComponent->BindAction(CardSlotActions[i], ETriggerEvent::Started, this, &AbebraPlayerController::OnCardSlotPressed,i);
				}
			}
			if (ConfirmCastAction)
			{
				EnhancedInputComponent->BindAction(ConfirmCastAction, ETriggerEvent::Started, this, &AbebraPlayerController::OnConfirmCast);
			}
			if (CancelCastAction)
			{
				EnhancedInputComponent->BindAction(CancelCastAction, ETriggerEvent::Started, this, &AbebraPlayerController::OnCancelCast);
			}
			UE_LOG(LogTemp, Warning, TEXT("Confirm=%s Cancel=%s"), *GetNameSafe(ConfirmCastAction), *GetNameSafe(CancelCastAction));
			if (MoveAction)
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AbebraPlayerController::OnMove);
			}
			if (SettingsAction)
			{
				EnhancedInputComponent->BindAction(SettingsAction, ETriggerEvent::Started, this, &AbebraPlayerController::OpenSettings);
			}
		}
		
		else
		{
			UE_LOG(Logbebra, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
		

	}
}

void AbebraPlayerController::OnInputStarted()
{
	StopMovement();

	// Update the move destination to wherever the cursor is pointing at
	UpdateCachedDestination();
}

void AbebraPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// Update the move destination to wherever the cursor is pointing at
	UpdateCachedDestination();
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AbebraPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AbebraPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AbebraPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AbebraPlayerController::UpdateCachedDestination()
{
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
}
UDeckComponent* AbebraPlayerController::GetDeck() const
{
	APawn* ControlledPawn = GetPawn();
	return ControlledPawn ? ControlledPawn->FindComponentByClass<UDeckComponent>() : nullptr;
}

void AbebraPlayerController::OnCardSlotPressed(int32 SlotIndex)
{
	if (bPlayerDead)
	{
		return;
	}
	
	UDeckComponent* Deck = GetDeck();
	if (!Deck || !Deck->GetHand().IsValidIndex(SlotIndex))
	{
		return;
	}
	
	SetSelectedIndex(SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Selected card %d"), SlotIndex);
	
	if (CastMode == ECastMode::Quick)
	{
		CastSelectedCard();
	}
}

void AbebraPlayerController::OnConfirmCast()
{
	if (bPlayerDead)
	{
		return;
	}
	CastSelectedCard();
}

void AbebraPlayerController::CastSelectedCard()
{
	UDeckComponent* Deck = GetDeck();
	if (!Deck || SelectedHandIndex == INDEX_NONE)
	{
		return;
	}
	
	const TArray<UCardData*> Hand = Deck->GetHand();
	if (!Hand.IsValidIndex(SelectedHandIndex))
	{
		SetSelectedIndex(INDEX_NONE);
		return;
	}	
	
	UCardData* Card = Hand[SelectedHandIndex];
	APawn* ControlledPawn = GetPawn();
	if (!Card || !ControlledPawn)
	{
		return;
	}
	
	FHitResult Hit;
	if  (!GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{
		UE_LOG(LogTemp, Warning, TEXT("No target under cursor"));
		return;
	}
	
	FVector TargetLocation = Hit.Location;
	
	if (Card->Range > 0.f)
	{
		const FVector Origin = ControlledPawn->GetActorLocation();
		FVector Offset = TargetLocation - Origin;
		Offset.Z = 0.f;
		
		if (Offset.Size() > Card->Range)
		{
			const FVector Clamped = Origin + Offset.GetSafeNormal() * Card->Range;
			TargetLocation = FVector(Clamped.X, Clamped.Y, Clamped.Z);
		}
	}
	
	UStatsComponent* Stats = ControlledPawn->FindComponentByClass<UStatsComponent>();
	if (!Stats)
	{
		UE_LOG(LogTemp, Warning, TEXT("No stats component on pawn"));
		return;
	}
	
	if (!Stats->TrySpendMana(Card->ManaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough mana for %s"), *Card->CardName.ToString());
		return;
	}
	
	Stats->ReceiveDamage(Card->SelfDamage);
	
	UE_LOG(LogTemp, Warning, TEXT("Cast %s at %s"), *Card->CardName.ToString(), *TargetLocation.ToString());
	
	FCardCastContext Context;
	Context.Caster = ControlledPawn;
	Context.Card = Card;
	Context.TargetLocation = TargetLocation;
	
	for (UCardEffect* Effect : Card->Effects)
	{
		if (Effect)
		{
			Effect->Execute(Context);
		}
	}
	
	Deck->PlayCard(SelectedHandIndex);
	SetSelectedIndex(INDEX_NONE);
}

void AbebraPlayerController::OnCancelCast()
{
	SetSelectedIndex(INDEX_NONE);
	UE_LOG(LogTemp, Warning, TEXT("Selection canselled"));
}

void AbebraPlayerController::OnMove(const FInputActionValue& Value)
{
	
	if (bPlayerDead)
	{
		return;
	}
	
	const FVector2D Input = Value.Get<FVector2D>();
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || !PlayerCameraManager)
	{
		return;
	}
	
	const FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f);
	
	const FVector Forward = YawRotation.RotateVector(FVector::ForwardVector);
	const FVector Righth = YawRotation.RotateVector(FVector::RightVector);
	
	ControlledPawn->AddMovementInput(Forward, Input.Y);
	ControlledPawn->AddMovementInput(Righth, Input.X);
}

void AbebraPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
	LoadCastSettings();
	
	if (HudWidgetClass && !HudWidget)
	{
		HudWidget = CreateWidget<UHudWidget>(this, HudWidgetClass);
		if (HudWidget)
		{
			HudWidget->AddToViewport();
		}
	}
	
	if (HudWidget)
	{
		HudWidget->InitForPawn(InPawn);
	}
	
	if (BoundStats)
	{
		BoundStats->OnDied.RemoveDynamic(this, &AbebraPlayerController::HandlePlayerDeath);
		BoundStats = nullptr;
	}
	
	bPlayerDead = false;
	BoundStats = InPawn ? InPawn->FindComponentByClass<UStatsComponent>() : nullptr;
	if (BoundStats)
	{
		BoundStats->OnDied.AddDynamic(this, &AbebraPlayerController::HandlePlayerDeath);
	}
}


void AbebraPlayerController::SetSelectedIndex(int32 NewIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("SetSelectedIndex(%d)"), NewIndex);
	SelectedHandIndex = NewIndex;
	OnCardSelectionChanged.Broadcast(SelectedHandIndex);
}

void AbebraPlayerController::HandlePlayerDeath()
{
	bPlayerDead = true;
	SetSelectedIndex(INDEX_NONE);
	StopMovement();
	SetIgnoreMoveInput(true);

	if (DeathScreenClass && !DeathScreenWidget)
	{
		DeathScreenWidget = CreateWidget<UDeathScreenWidget>(this, DeathScreenClass);
		if (DeathScreenWidget)
		{
			DeathScreenWidget->OnRestartRequested.AddDynamic(this, &AbebraPlayerController::RestartCurrentLevel);
			DeathScreenWidget->AddToViewport(100);
		}
	}

	if (DeathScreenWidget)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(DeathScreenWidget->TakeWidget());
		SetInputMode(InputMode);
	}
	else
	{
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AbebraPlayerController::RestartCurrentLevel, RestartDelay, false);
	}
}

void AbebraPlayerController::RestartCurrentLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

static const FString CastSettingsSlot = TEXT("Settings");

void AbebraPlayerController::LoadCastSettings()
{
	if (!UGameplayStatics::DoesSaveGameExist(CastSettingsSlot, 0))
	{
		return;
	}

	if (UGameSettingsSave* Save = Cast<UGameSettingsSave>(UGameplayStatics::LoadGameFromSlot(CastSettingsSlot, 0)))
	{
		CastMode = Save->bQuickCast ? ECastMode::Quick : ECastMode::Normal;
	}
}

void AbebraPlayerController::SaveCastSettings()
{
	UGameSettingsSave* Save = Cast<UGameSettingsSave>(UGameplayStatics::CreateSaveGameObject(UGameSettingsSave::StaticClass()));
	if (Save)
	{
		Save->bQuickCast = (CastMode == ECastMode::Quick);
		UGameplayStatics::SaveGameToSlot(Save, CastSettingsSlot, 0);
	}
}

void AbebraPlayerController::OpenSettings()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenSettings: dead=%d widget=%s class=%s"),
	bPlayerDead ? 1 : 0, *GetNameSafe(SettingsWidget), *GetNameSafe(SettingsWidgetClass.Get()));
	
	if (bPlayerDead || SettingsWidget || !SettingsWidgetClass)
	{
		return;
	}

	SettingsWidget = CreateWidget<USettingsWidget>(this, SettingsWidgetClass);
	if (!SettingsWidget)
	{
		return;
	}

	SettingsWidget->SetQuickCast(CastMode == ECastMode::Quick);
	SettingsWidget->OnQuickCastToggled.AddDynamic(this, &AbebraPlayerController::HandleQuickCastToggled);
	SettingsWidget->OnCloseRequested.AddDynamic(this, &AbebraPlayerController::CloseSettings);
	SettingsWidget->AddToViewport(50);

	SetSelectedIndex(INDEX_NONE);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(SettingsWidget->TakeWidget());
	SetInputMode(InputMode);

	UGameplayStatics::SetGamePaused(this, true);
}

void AbebraPlayerController::CloseSettings()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (SettingsWidget)
	{
		SettingsWidget->RemoveFromParent();
		SettingsWidget = nullptr;
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AbebraPlayerController::HandleQuickCastToggled(bool bQuickCast)
{
	CastMode = bQuickCast ? ECastMode::Quick : ECastMode::Normal;
	SaveCastSettings();
}


#pragma once

#include "CoreMinimal.h"
//#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "DeckComponent.h"
#include "CardData.h"
#include "Engine/TimerHandle.h"
#include "bebraPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UPathFollowingComponent;
class UHudWidget;
class UStatsComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECastMode : uint8
{
	Normal,
	Quick
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AbebraPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UHudWidget> HudWidgetClass;

	UPROPERTY()
	TObjectPtr<UHudWidget> HudWidget;

	virtual void OnPossess(APawn* InPawn) override;


	/** Component used for moving along a NavMesh path. */
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	TObjectPtr<UPathFollowingComponent> PathFollowingComponent;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, Category="Input")
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UNiagaraSystem> FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SetDestinationTouchAction;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Set to true if we're using touch input */
	uint32 bIsTouch : 1;

	/** Saved location of the character movement destination */
	FVector CachedDestination;

	/** Time that the click input has been pressed */
	float FollowTime = 0.0f;
	
	int32 SelectedHandIndex = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, Category="Cards|Input")
	TArray<TObjectPtr<UInputAction>> CardSlotActions;
	
	UPROPERTY(EditAnywhere, Category="Cards|Input")
	TObjectPtr<UInputAction> ConfirmCastAction;
	
	UPROPERTY(EditAnywhere, Category="Cards|Input")
	TObjectPtr<UInputAction> CancelCastAction;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardSelectionChanged, int32, SelectedHandIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Cards")
	ECastMode CastMode = ECastMode::Normal;
	
	int32 SelectedCardIndex = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void OnMove(const FInputActionValue& Value);

public:

	/** Constructor */
	AbebraPlayerController();
	
	UPROPERTY(BlueprintAssignable, Category="Cards")
	FOnCardSelectionChanged OnCardSelectionChanged;

protected:

	/** Initialize input bindings */
	virtual void SetupInputComponent() override;
	
	/** Input handlers */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	
	/** Helper function to get the move destination */
	void UpdateCachedDestination();
	void OnCardSlotPressed(int32 SlotIndex);
	void OnConfirmCast();
	void OnCancelCast();
	void CastSelectedCard();
	UDeckComponent* GetDeck() const;
	void SetSelectedIndex(int32 NewIndex);
	
	UFUNCTION()
	void HandlePlayerDeath();
	
	void RestartLevel();
	
	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = "0"))
	float RestartDelay = 5.0f;
	
	bool bPlayerDead = false;
	
	FTimerHandle RestartTimer;
	
	UPROPERTY()
	TObjectPtr<UStatsComponent> BoundStats;
};



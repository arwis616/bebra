#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HandWidget.generated.h"

class APawn;
class UHorizontalBox;
class UDeckComponent;
class UCardWidget;
class AbebraPlayerController;

UCLASS(Abstract)
class BEBRA_API UHandWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitForPawn(APawn* Pawn);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardBox;
	
	UPROPERTY(EditAnywhere, Category = "Cards")
	TSubclassOf<UCardWidget> CardWidgetClass;
	
	UFUNCTION()
	void RefreshHand();
	
	UFUNCTION()
	void HandleSelectionChanged(int32 NewSelection);

private:
	UPROPERTY()
	TObjectPtr<UDeckComponent> Deck;

	void ApplySelection();

	UPROPERTY()
	TObjectPtr<AbebraPlayerController> Controller;

	int32 CurrentSelection = INDEX_NONE;
};
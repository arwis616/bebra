#include "HandWidget.h"
#include "bebraPlayerController.h"
#include "CardWidget.h"
#include "CardData.h"
#include "DeckComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "GameFramework/Pawn.h"

void UHandWidget::InitForPawn(APawn* Pawn)
{
	if (Controller)
	{
		Controller->OnCardSelectionChanged.RemoveDynamic(this, &UHandWidget::HandleSelectionChanged);
		Controller = nullptr;
	}

	if (Deck)
	{
		Deck->OnHandChanged.RemoveDynamic(this, &UHandWidget::RefreshHand);
		Deck = nullptr;
	}

	if (!Pawn)
	{
		return;
	}

	Deck = Pawn->FindComponentByClass<UDeckComponent>();
	Controller = Cast<AbebraPlayerController>(Pawn->GetController());
    UE_LOG(LogTemp, Warning, TEXT("HandWidget controller=%s"), *GetNameSafe(Controller));
	if (Controller)
	{
		Controller->OnCardSelectionChanged.AddDynamic(this, &UHandWidget::HandleSelectionChanged);
	}

	if (Deck)
	{
		Deck->OnHandChanged.AddDynamic(this, &UHandWidget::RefreshHand);
		RefreshHand();
	}
}

void UHandWidget::RefreshHand()
{
	UE_LOG(LogTemp, Warning, TEXT("RefreshHand: deck=%s box=%s class=%s"),
		*GetNameSafe(Deck), *GetNameSafe(CardBox), *GetNameSafe(CardWidgetClass.Get()));
	if (!Deck || !CardBox || !CardWidgetClass)
	{
		return;
	}

	CardBox->ClearChildren();

	for (UCardData* Card : Deck->GetHand())
	{
		UCardWidget* CardWidget = CreateWidget<UCardWidget>(this, CardWidgetClass);
		if (!CardWidget)
		{
			continue;
		}

		CardWidget->SetCard(Card);

		if (UHorizontalBoxSlot* BoxSlot = CardBox->AddChildToHorizontalBox(CardWidget))
		{
			BoxSlot->SetPadding(FMargin(8.f, 0.f));
		}
	}

	ApplySelection();
}

void UHandWidget::HandleSelectionChanged(int32 NewSelection)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleSelectionChanged(%d)"), NewSelection);
	CurrentSelection = NewSelection;
	ApplySelection();
}

void UHandWidget::ApplySelection()
{
    if (!CardBox)
    {
       return;
    }

    UE_LOG(LogTemp, Warning, TEXT("ApplySelection: children=%d selected=%d"), CardBox->GetChildrenCount(), CurrentSelection);

    for (int32 i = 0; i < CardBox->GetChildrenCount(); ++i)
    {
       if (UCardWidget* CardWidget = Cast<UCardWidget>(CardBox->GetChildAt(i)))
       {
          CardWidget->SetSelected(i == CurrentSelection);
       }
    }
}
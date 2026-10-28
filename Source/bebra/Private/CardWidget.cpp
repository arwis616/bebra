#include "CardWidget.h"
#include "CardData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCardWidget::SetCard(UCardData* Card)
{
	if (!Card)
	{
		return;
	}

	if (CardNameText)
	{
		CardNameText->SetText(Card->CardName);
	}
	if (ManaCostText)
	{
		ManaCostText->SetText(FText::AsNumber(Card->ManaCost));
	}
	if (ArtImage && Card->Art)
	{
		ArtImage->SetBrushFromTexture(Card->Art);
	}
}

void UCardWidget::SetSelected(bool bSelected)
{
	SetRenderTranslation(FVector2D(0.f, bSelected ? -30.f : 0.f));
}
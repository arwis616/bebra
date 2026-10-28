#include "DeckComponent.h"
#include "CardData.h"

UDeckComponent::UDeckComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDeckComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Deck.Reset();
	for (UCardData* Card : StartingCards)
	{
	    if (Card)
	    {
	        Deck.Add(Card);
	    }
	}
	
	ShuffleDeck();
	FillHand();
}

void UDeckComponent::ShuffleDeck()
{
	for (int32 i = Deck.Num() - 1; i > 0; --i)
	{
		const int32 j = FMath::RandRange(0, i);
		Deck.Swap(i, j);
	}
}

bool UDeckComponent::DrawCard()
{
	if (Deck.Num() == 0 || Hand.Num() >= MaxHandSize)
	{
		return false;
	}
	Hand.Add(Deck[0]);
	Deck.RemoveAt(0);
	return true;
}


void UDeckComponent::FillHand()
{
	while (DrawCard())
	{
		
	}
	OnHandChanged.Broadcast();

	for (int32 i = 0; i < Hand.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hand[%d] = %s"), i, *Hand[i]->CardName.ToString());
	}
}

bool UDeckComponent::PlayCard(int32 HandIndex)
{
	if (!Hand.IsValidIndex(HandIndex))
	{
		return false;
	}

	UCardData* Card = Hand[HandIndex];
	Hand.RemoveAt(HandIndex);
	Deck.Add(Card);
	FillHand();
	return true;
}

TArray<UCardData*> UDeckComponent::GetHand() const
{
	TArray<UCardData*> Result;
	Result.Reserve(Hand.Num());
	for (UCardData* Card : Hand)
	{
		Result.Add(Card);
	}
	return Result;
}
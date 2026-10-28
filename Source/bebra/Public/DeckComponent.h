#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeckComponent.generated.h"

class UCardData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandChanged);

UCLASS(ClassGroup = (Cards), meta = (BlueprintSpawnableComponent))
class BEBRA_API UDeckComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UDeckComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deck")
	TArray<TObjectPtr<UCardData>> StartingCards;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deck", meta = (ClampMin = 1))
	int32 MaxHandSize = 5;
	
	UPROPERTY(BlueprintAssignable, Category = "Deck")
	FOnHandChanged OnHandChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Deck")
	bool PlayCard(int32 HandIndex);
	
	UFUNCTION(BlueprintPure, Category = "Deck")
	TArray<UCardData*> GetHand() const;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UCardData>> Deck;
	
	UPROPERTY()
	TArray<TObjectPtr<UCardData>> Hand;
	
	void ShuffleDeck();
	bool DrawCard();
	void FillHand();
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardWidget.generated.h"

class UCardData;
class UTextBlock;
class UImage;

UCLASS(Abstract)
class BEBRA_API UCardWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    void SetCard(UCardData* CardData);
	void SetSelected(bool bSelected);
    
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CardNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaCostText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ArtImage;
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

class UHandWidget;
class APawn;
class UProgressBar;
class UStatsComponent;

UCLASS(ABSTRACT)
class BEBRA_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitForPawn(APawn* Pawn);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHandWidget> HandWidget;
	
	UFUNCTION()
	void RefreshStats();
	
private:
	UPROPERTY()
	TObjectPtr<UStatsComponent> Stats;
};
	
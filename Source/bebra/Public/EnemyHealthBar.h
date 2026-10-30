#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;
class UStatsComponent;

UCLASS(Abstract)
class BEBRA_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitForStats(UStatsComponent* InStats);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UFUNCTION()
	void Refresh();

private:
	UPROPERTY()
	TObjectPtr<UStatsComponent> Stats;
};
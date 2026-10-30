#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathScreenWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartRequested);

UCLASS(Abstract)
class BEBRA_API UDeathScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Death")
	FOnRestartRequested OnRestartRequested;

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UFUNCTION()
	void HandleRestartClicked();
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UCheckBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickCastToggled, bool, bQuickCast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsCloseRequested);

UCLASS(Abstract)
class BEBRA_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnQuickCastToggled OnQuickCastToggled;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnSettingsCloseRequested OnCloseRequested;

	void SetQuickCast(bool bQuickCast);

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> QuickCastCheck;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UFUNCTION()
	void HandleCheckChanged(bool bIsChecked);

	UFUNCTION()
	void HandleCloseClicked();
};
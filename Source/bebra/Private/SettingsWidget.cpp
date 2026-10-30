#include "SettingsWidget.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"

void USettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (QuickCastCheck)
	{
		QuickCastCheck->OnCheckStateChanged.AddDynamic(this, &USettingsWidget::HandleCheckChanged);
	}
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleCloseClicked);
	}
}

void USettingsWidget::SetQuickCast(bool bQuickCast)
{
	if (QuickCastCheck)
	{
		QuickCastCheck->SetIsChecked(bQuickCast);
	}
}

void USettingsWidget::HandleCheckChanged(bool bIsChecked)
{
	OnQuickCastToggled.Broadcast(bIsChecked);
}

void USettingsWidget::HandleCloseClicked()
{
	OnCloseRequested.Broadcast();
}
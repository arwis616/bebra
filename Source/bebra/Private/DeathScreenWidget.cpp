#include "DeathScreenWidget.h"
#include "Components/Button.h"

void UDeathScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UDeathScreenWidget::HandleRestartClicked);
	}
}

void UDeathScreenWidget::HandleRestartClicked()
{
	OnRestartRequested.Broadcast();
}
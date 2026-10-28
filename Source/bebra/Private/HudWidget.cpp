#include "HudWidget.h"
#include "HandWidget.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Pawn.h"
#include "StatsComponent.h"

void UHudWidget::InitForPawn(APawn* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("HudWidget init: hand=%s"), *GetNameSafe(HandWidget));

	if (HandWidget)
	{
		HandWidget->InitForPawn(Pawn);
	}

	if (Stats)
	{
		Stats->OnStatsChanged.RemoveDynamic(this, &UHudWidget::RefreshStats);
		Stats = nullptr;
	}

	if (!Pawn)
	{
		return;
	}

	Stats = Pawn->FindComponentByClass<UStatsComponent>();
	if (Stats)
	{
		Stats->OnStatsChanged.AddDynamic(this, &UHudWidget::RefreshStats);
		RefreshStats();
	}
}

void UHudWidget::RefreshStats()
{
	if (!Stats)
	{
		return;
	}

	if (HealthBar)
	{
		HealthBar->SetPercent(Stats->GetHealth() / Stats->MaxHealth);
	}
	if (ManaBar)
	{
		ManaBar->SetPercent(Stats->GetMana() / Stats->MaxMana);
	}
}
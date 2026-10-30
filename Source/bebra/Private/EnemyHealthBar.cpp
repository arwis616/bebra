#include "EnemyHealthBar.h"
#include "Components/ProgressBar.h"
#include "StatsComponent.h"

void UEnemyHealthBar::InitForStats(UStatsComponent* InStats)
{
	if (Stats)
	{
		Stats->OnStatsChanged.RemoveDynamic(this, &UEnemyHealthBar::Refresh);
	}

	Stats = InStats;

	if (Stats)
	{
		Stats->OnStatsChanged.AddDynamic(this, &UEnemyHealthBar::Refresh);
		Refresh();
	}
}

void UEnemyHealthBar::Refresh()
{
	if (!Stats || !HealthBar)
	{
		return;
	}

	const float Percent = Stats->GetHealth() / Stats->MaxHealth;
	HealthBar->SetPercent(Percent);

	SetVisibility(Percent >= 1.f ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}
#include "HealEffect.h"
#include "StatsComponent.h"
#include "GameFramework/Actor.h"

void UHealEffect::Execute(const FCardCastContext& Context)
{
	if (!Context.Caster)
	{
		return;
	}
	
	if (UStatsComponent* Stats = Context.Caster->FindComponentByClass<UStatsComponent>())
	{
		Stats->Heal(Amount);
	}
}

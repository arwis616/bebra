#include "StatsComponent.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Mana = MaxMana;
	bDead = false;
	OnStatsChanged.Broadcast();
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mana < MaxMana)
	{
		Mana = FMath::Min(Mana + ManaRegenPerSecond * DeltaTime, MaxMana);
		OnStatsChanged.Broadcast();
	}
}

bool UStatsComponent::TrySpendMana(float Amount)
{
	if (Mana < Amount)
	{
		return false;
	}

	Mana -= Amount;
	OnStatsChanged.Broadcast();
	return true;
}

void UStatsComponent::ReceiveDamage(float Amount)
{
	if (bDead || Amount <= 0.f)
	{
		return;
	}

	Health = FMath::Max(Health - Amount, 0.f);
	OnStatsChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Health: %.1f / %.1f"), Health, MaxHealth);

	if (Health <= 0.f)
	{
		bDead = true;
		OnDied.Broadcast();
	}
}
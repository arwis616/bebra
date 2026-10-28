#include "EnemyDummy.h"
#include "StatsComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

AEnemyDummy::AEnemyDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Stats = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));
}

void AEnemyDummy::BeginPlay()
{
	Super::BeginPlay();

	Stats->OnDied.AddDynamic(this, &AEnemyDummy::HandleDeath);
}

void AEnemyDummy::OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator)
{
	if (Element == ECardElement::None || Stats->IsDead())
	{
		return;
	}

	const float* Multiplier = DamageMultipliers.Find(Element);
	const float Damage = Power * (Multiplier ? *Multiplier : 1.f);

	Stats->ReceiveDamage(Damage);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 150.f),
		FString::Printf(TEXT("%.0f / %.0f"), Stats->GetHealth(), Stats->MaxHealth),
		nullptr, FColor::Red, 1.5f);
}

void AEnemyDummy::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *GetName());
	Destroy();
}
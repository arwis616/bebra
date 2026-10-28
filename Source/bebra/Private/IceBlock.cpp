#include "IceBlock.h"

AIceBlock::AIceBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AIceBlock::BeginPlay()
{
	Super::BeginPlay();
	Durability = MaxDurability;
}

void AIceBlock::OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator)
{
	if (Element != ECardElement::Fire)
	{
		return;
	}

	Durability -= Power;
	UE_LOG(LogTemp, Warning, TEXT("IceBlock melts: %.1f / %.1f"), FMath::Max(Durability, 0.f), MaxDurability);

	if (Durability <= 0.f)
	{
		Destroy();
		return;
	}

	SetActorScale3D(FVector(Durability / MaxDurability));
}
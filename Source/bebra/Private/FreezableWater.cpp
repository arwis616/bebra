#include "FreezableWater.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

AFreezableWater::AFreezableWater()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AFreezableWater::BeginPlay()
{
	Super::BeginPlay();
	SetFrozen(false);
}

void AFreezableWater::OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator)
{
	if (Element == ECardElement::Ice && !bFrozen)
	{
		SetFrozen(true);
	}
	else if (Element == ECardElement::Fire && bFrozen)
	{
		SetFrozen(false);
	}
}

void AFreezableWater::SetFrozen(bool bNewFrozen)
{
	bFrozen = bNewFrozen;

	Mesh->SetCollisionResponseToChannel(ECC_Pawn, bFrozen ? ECR_Ignore : ECR_Block);

	UMaterialInterface* NewMaterial = bFrozen ? FrozenMaterial : LiquidMaterial;
	if (NewMaterial)
	{
		Mesh->SetMaterial(0, NewMaterial);
	}

	UE_LOG(LogTemp, Warning, TEXT("Water is now %s"), bFrozen ? TEXT("frozen") : TEXT("liquid"));
}
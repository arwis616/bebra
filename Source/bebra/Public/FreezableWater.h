#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementReactive.h"
#include "FreezableWater.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class BEBRA_API AFreezableWater : public AActor, public IElementReactive
{
	GENERATED_BODY()

public:
	AFreezableWater();

	virtual void OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Water")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Water")
	TObjectPtr<UMaterialInterface> LiquidMaterial;

	UPROPERTY(EditAnywhere, Category = "Water")
	TObjectPtr<UMaterialInterface> FrozenMaterial;

private:
	void SetFrozen(bool bNewFrozen);

	bool bFrozen = false;
};
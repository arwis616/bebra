#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementReactive.h"
#include "IceBlock.generated.h"

class UStaticMeshComponent;

UCLASS()
class BEBRA_API AIceBlock : public AActor, public IElementReactive
{
	GENERATED_BODY()

public:
	AIceBlock();

	virtual void OnHitByElement_Implementation(ECardElement Element, float Power, AActor* Instigator) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ice")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Ice", meta = (ClampMin = "1"))
	float MaxDurability = 15.f;

private:
	float Durability = 0.f;
};
#pragma once

#include "CoreMinimal.h"
#include "CardEffect.h"
#include "AreaElementEffect.generated.h"

UCLASS(meta = (DisplayName = "Area Element Effect"))
class BEBRA_API UAreaElementEffect : public UCardEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = "1"))
	float Radius = 250.f;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = "0"))
	float Power = 10.f;

	virtual void Execute(const FCardCastContext& Context) override;
};
#pragma once

#include "CoreMinimal.h"
#include "CardEffect.h"
#include "UObject/Object.h"
#include "HealEffect.generated.h"

UCLASS(meta = (DisplayName = "Heal Effect"))
class BEBRA_API UHealEffect : public UCardEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = "0"))
	float Amount = 15.f;
	
	virtual void Execute(const FCardCastContext& Context) override;
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CardEffect.generated.h"

class AActor;
class UCardData;

USTRUCT(BlueprintType)
struct FCardCastContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Caster = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCardData> Card = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation = FVector::ZeroVector;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class BEBRA_API UCardEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute(const FCardCastContext& Context) {}
};
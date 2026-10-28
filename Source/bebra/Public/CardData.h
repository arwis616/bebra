#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CardData.generated.h"

class UTexture2D;
class UCardEffect;

UENUM(BlueprintType)
enum class ECardElement : uint8
{
	None,
	Fire,
	Ice,
	Lighting
};

UCLASS(BlueprintType)
class BEBRA_API UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText CardName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	ECardElement Element = ECardElement::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (ClampMin = 0))
	int32 ManaCost = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (ClampMin = 0))
	float SelfDamage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TObjectPtr<UTexture2D> Art;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Card")
	TArray<TObjectPtr<UCardEffect>> Effects;
};


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardData.h"
#include "ElementReactive.generated.h"

class AActor;

UINTERFACE(MinimalAPI, BlueprintType)
class UElementReactive : public UInterface
{
	GENERATED_BODY()
};

class BEBRA_API IElementReactive
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Element")
		void OnHitByElement(ECardElement Element, float Power, AActor* Instigator);
};

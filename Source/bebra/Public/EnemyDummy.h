#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementReactive.h"
#include "EnemyDummy.generated.h"

class UStaticMeshComponent;
class UStatsComponent;

UCLASS()
class BEBRA_API AEnemyDummy : public AActor, public IElementReactive
{
	GENERATED_BODY()

public:
	AEnemyDummy();

	virtual void OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UStatsComponent> Stats;
	
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TMap<ECardElement, float> DamageMultipliers;
};
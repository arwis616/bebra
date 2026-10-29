#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);

UCLASS(ClassGroup = (Cards), meta = (BlueprintSpawnableComponent))
class BEBRA_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "1"))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "1"))
	float MaxMana = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0"))
	float ManaRegenPerSecond = 0.5f;

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatsChanged OnStatsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnDied OnDied;

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetMana() const { return Mana; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	bool IsDead() const { return bDead; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool TrySpendMana(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ReceiveDamage(float Amount);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Health = 0.f;
	float Mana = 0.f;
	bool bDead = false;
};
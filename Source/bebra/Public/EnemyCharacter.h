#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ElementReactive.h"
#include "EnemyCharacter.generated.h"

class UStatsComponent;
class UWidgetComponent;

UCLASS()
class BEBRA_API AEnemyCharacter : public ACharacter, public IElementReactive
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDeath();

	void EndSlow();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UStatsComponent> Stats;
	
	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	TMap<ECardElement, float> DamageMultipliers;

	UPROPERTY(EditAnywhere, Category = "Enemy|Movement", meta = (ClampMin = "0"))
	float ChaseSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|Movement", meta = (ClampMin = "0"))
	float AggroRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat", meta = (ClampMin = "0"))
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat", meta = (ClampMin = "0"))
	float AttackDamage = 5.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat", meta = (ClampMin = "0.1"))
	float AttackCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy|Ice", meta = (ClampMin = "0.05", ClampMax = "1"))
	float SlowMultiplier = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Enemy|Ice", meta = (ClampMin = "0"))
	float SlowDuration = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UWidgetComponent> HealthBarComponent;
	
private:
	FTimerHandle SlowTimer;
	float LastAttackTime = -1000.f;
};
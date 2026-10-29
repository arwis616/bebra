#include "EnemyCharacter.h"
#include "StatsComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	Stats->OnDied.AddDynamic(this, &AEnemyCharacter::HandleDeath);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Stats->IsDead())
	{
		return;
	}

	APawn* Target = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Target)
	{
		return;
	}

	UStatsComponent* TargetStats = Target->FindComponentByClass<UStatsComponent>();
	if (TargetStats && TargetStats->IsDead())
	{
		return;
	}

	FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.f;
	const float Distance = ToTarget.Size();

	if (Distance > AggroRange)
	{
		return;
	}

	if (Distance > AttackRange)
	{
		AddMovementInput(ToTarget.GetSafeNormal(), 1.f);
		return;
	}

	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastAttackTime >= AttackCooldown)
	{
		LastAttackTime = Now;

		if (TargetStats)
		{
			TargetStats->ReceiveDamage(AttackDamage);
		}
	}
}

void AEnemyCharacter::OnHitByElement_Implementation(ECardElement Element, float Power, AActor* HitInstigator)
{
	if (Element == ECardElement::None || Stats->IsDead())
	{
		return;
	}

	const float* Multiplier = DamageMultipliers.Find(Element);
	Stats->ReceiveDamage(Power * (Multiplier ? *Multiplier : 1.f));

	if (Element == ECardElement::Ice && !Stats->IsDead())
	{
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed * SlowMultiplier;
		GetWorldTimerManager().SetTimer(SlowTimer, this, &AEnemyCharacter::EndSlow, SlowDuration, false);
	}

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 150.f),
		FString::Printf(TEXT("%.0f / %.0f"), Stats->GetHealth(), Stats->MaxHealth),
		nullptr, FColor::Red, 1.5f);
}

void AEnemyCharacter::EndSlow()
{
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AEnemyCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *GetName());
	Destroy();
}
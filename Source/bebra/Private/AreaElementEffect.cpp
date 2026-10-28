#include "AreaElementEffect.h"
#include "CardData.h"
#include "ElementReactive.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Actor.h"

void UAreaElementEffect::Execute(const FCardCastContext& Context)
{
	if (!Context.Caster || !Context.Card)
	{
		return;
	}

	UWorld* World = Context.Caster->GetWorld();
	if (!World)
	{
		return;
	}

	DrawDebugSphere(World, Context.TargetLocation, Radius, 24, FColor::Orange, false, 1.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Caster);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(
		Overlaps,
		Context.TargetLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllObjects),
		FCollisionShape::MakeSphere(Radius),
		Params);

	TSet<AActor*> Handled;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Actor = Overlap.GetActor();
		if (!Actor || Handled.Contains(Actor))
		{
			continue;
		}
		Handled.Add(Actor);

		if (Actor->Implements<UElementReactive>())
		{
			IElementReactive::Execute_OnHitByElement(Actor, Context.Card->Element, Power, Context.Caster);
		}
	}
}
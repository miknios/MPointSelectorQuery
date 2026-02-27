// Copyright (c) Miknios. All rights reserved.

#include "MPointSelectorQueryFilter.h"

#include "MSelectorQueryPoint.h"

bool UMPointSelectorQueryFilter_Base::FilterPoint_Impl_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const
{
	return true;
}

bool UMPointSelectorQueryFilter_Base::FilterPoint(const UObject* ContextObject,
                                                  const FVector& ReferenceLocation,
                                                  const FVector& ReferenceDirection,
                                                  const UMPointSelectorQuerySettingsAsset* QuerySettings,
                                                  const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const
{
	const bool FilterResult = FilterPoint_Impl(ContextObject, ReferenceLocation, ReferenceDirection, QuerySettings, LookAtPoint);
	return bInvert ? !FilterResult : FilterResult;
}

bool UMPointSelectorQueryFilter_LineOfSight::FilterPoint_Impl_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const
{
	if (QuerySettings == nullptr)
	{
		return false;
	}

	if (!IsValid(LookAtPoint.GetObject()))
	{
		return false;
	}

	const UWorld* World = ContextObject->GetWorld();
	if (!IsValid(World))
	{
		return false;
	}

	const AActor* ActorOwner = IMSelectorQueryPointInterface::Execute_GetActorOwner(LookAtPoint.GetObject());
	if (!IsValid(ActorOwner))
	{
		return false;
	}

	const FVector TraceStart = ReferenceLocation;
	const FVector TraceEnd = IMSelectorQueryPointInterface::Execute_GetLookAtLocation(LookAtPoint.GetObject());

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Cast<AActor>(ContextObject));

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		TraceChannel,
		CollisionQueryParams
	);

	if (!bHit)
	{
		return false;
	}

	const bool bPointActorHit = HitResult.GetActor() == ActorOwner;
	return bPointActorHit;
}

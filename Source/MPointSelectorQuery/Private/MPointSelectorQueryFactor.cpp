// Copyright (c) Miknios. All rights reserved.

#include "MPointSelectorQueryFactor.h"

#include "MMath.h"
#include "MPointSelectorQueryLibrary.h"
#include "MPointSelectorQuerySettingsAsset.h"

float UMPointSelectorQueryFactor_Base::CalculateFactorValueForLookAtPoint_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
{
	return 0.f;
}

float UMPointSelectorQueryFactor_Distance::CalculateFactorValueForLookAtPoint_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
{
	if (QuerySettings == nullptr)
	{
		return 0.f;
	}

	const FVector LookAtTo = LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject());
	const float Distance = FVector::Distance(ReferenceLocation, LookAtTo);
	const float DistanceMax = QuerySettings->DistanceMax;
	const float Result = 1 - FMath::Clamp(Distance / DistanceMax, 0, 1);
	return Result;
}

float UMPointSelectorQueryFactor_Angle::CalculateFactorValueForLookAtPoint_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
{
	if (QuerySettings == nullptr)
	{
		return 0.f;
	}

	const FVector ToLookAtPointVector =
		MMath::FromToVector(ReferenceDirection,
		                    LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()));
	const float Dot = FVector::DotProduct(ReferenceDirection.GetSafeNormal(), ToLookAtPointVector);
	const float Result = FMath::Clamp(Dot, 0, 1);
	return Result;
}

float UMPointSelectorQueryFactor_AngleFlat::CalculateFactorValueForLookAtPoint_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
{
	if (QuerySettings == nullptr)
	{
		return 0.f;
	}

	const FVector ToLookAtPointVector =
		MMath::FromToVector(ReferenceLocation,
		                    LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()));

	const FVector ReferenceDirFlat = FVector(ReferenceDirection.X, ReferenceDirection.Y, 0).GetSafeNormal();
	const FVector ToLookAtPointFlat = FVector(ToLookAtPointVector.X, ToLookAtPointVector.Y, 0).GetSafeNormal();
	const float Dot = FVector::DotProduct(ReferenceDirFlat, ToLookAtPointFlat);
	const float Result = FMath::Clamp(Dot, 0, 1);
	return Result;
}

float UMPointSelectorQueryFactor_PointImportance::CalculateFactorValueForLookAtPoint_Implementation(
	const UObject* ContextObject,
	const FVector& ReferenceLocation,
	const FVector& ReferenceDirection,
	const UMPointSelectorQuerySettingsAsset* QuerySettings,
	const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
{
	return LookAtPoint->Execute_GetImportanceValue(LookAtPoint.GetObject());
}

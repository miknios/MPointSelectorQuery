// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MPointSelectorQueryFactor.generated.h"

class IMSelectorQueryPointInterface;
class UMPointSelectorQuerySettingsAsset;

UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MPOINTSELECTORQUERY_API UMPointSelectorQueryFactor_Base : public UObject
{
	GENERATED_BODY()

public:
	// Returns value between 0-1 that influence selecting given LookAtPoint as a point we look at
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Point Selector Query")
	float CalculateFactorValueForLookAtPoint(const UObject* ContextObject,
	                                         const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                                         const UMPointSelectorQuerySettingsAsset* QuerySettings,
	                                         const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint);
};

UCLASS(DisplayName = "Distance")
class UMPointSelectorQueryFactor_Distance : public UMPointSelectorQueryFactor_Base
{
	GENERATED_BODY()

public:
	virtual float CalculateFactorValueForLookAtPoint_Implementation(
		const UObject* ContextObject, const FVector& ReferenceLocation,
		const FVector& ReferenceDirection,
		const UMPointSelectorQuerySettingsAsset* QuerySettings,
		const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) override;
};

UCLASS(DisplayName = "Angle")
class UMPointSelectorQueryFactor_Angle : public UMPointSelectorQueryFactor_Base
{
	GENERATED_BODY()

public:
	virtual float CalculateFactorValueForLookAtPoint_Implementation(
		const UObject* ContextObject, const FVector& ReferenceLocation,
		const FVector& ReferenceDirection,
		const UMPointSelectorQuerySettingsAsset* QuerySettings,
		const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) override;
};

UCLASS(DisplayName = "Angle (Flat)")
class UMPointSelectorQueryFactor_AngleFlat : public UMPointSelectorQueryFactor_Base
{
	GENERATED_BODY()

public:
	virtual float CalculateFactorValueForLookAtPoint_Implementation(
		const UObject* ContextObject, const FVector& ReferenceLocation,
		const FVector& ReferenceDirection,
		const UMPointSelectorQuerySettingsAsset* QuerySettings,
		const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) override;
};

UCLASS(DisplayName = "POI Importance")
class UMPointSelectorQueryFactor_PointImportance : public UMPointSelectorQueryFactor_Base
{
	GENERATED_BODY()

public:
	virtual float CalculateFactorValueForLookAtPoint_Implementation(
		const UObject* ContextObject, const FVector& ReferenceLocation,
		const FVector& ReferenceDirection,
		const UMPointSelectorQuerySettingsAsset* QuerySettings,
		const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) override;
};

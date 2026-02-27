// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MPointSelectorQueryFilter.generated.h"

class UMPointSelectorQuerySettingsAsset;
class IMSelectorQueryPointInterface;

UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MPOINTSELECTORQUERY_API UMPointSelectorQueryFilter_Base : public UObject
{
	GENERATED_BODY()

public:
	bool FilterPoint(const UObject* ContextObject,
	                 const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                 const UMPointSelectorQuerySettingsAsset* QuerySettings,
	                 const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Point Selector Query")
	bool FilterPoint_Impl(const UObject* ContextObject,
	                      const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                      const UMPointSelectorQuerySettingsAsset* QuerySettings,
	                      const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const;

	UPROPERTY(EditAnywhere, Category = "Filter")
	bool bInvert = false;
};

UCLASS(DisplayName = "Is In Line Of Sight")
class UMPointSelectorQueryFilter_LineOfSight : public UMPointSelectorQueryFilter_Base
{
	GENERATED_BODY()

protected:
	// UMPointSelectorQueryFilter_Base
	virtual bool FilterPoint_Impl_Implementation(const UObject* ContextObject,
	                                             const FVector& ReferenceLocation,
	                                             const FVector& ReferenceDirection,
	                                             const UMPointSelectorQuerySettingsAsset* QuerySettings,
	                                             const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint) const override;
	// ~ UMPointSelectorQueryFilter_Base

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter", meta=(DisplayName="Trace Channel"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
};

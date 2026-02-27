// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MPointSelectorQueryFactor.h"
#include "MSelectorQueryPoint.h"
#include "MPointSelectorQueryLibrary.generated.h"

USTRUCT(BlueprintType)
struct FMPointSelectorQueryScoredPointData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IMSelectorQueryPointInterface> LookAtPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score = 0;
};

UCLASS()
class MPOINTSELECTORQUERY_API UMPointSelectorQueryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool PointSelectorQuery(const UObject* WorldContextObject,
	                               const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                               UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                               const FCollisionQueryParams& QueryParams, TScriptInterface<IMSelectorQueryPointInterface>& OutLookAtPoint);

	static TArray<FMPointSelectorQueryScoredPointData> QueryPoints(const UObject* WorldContextObject, const FVector& ReferenceLocation,
	                                                               const FVector& ReferenceDirection,
	                                                               UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                                                               const FCollisionQueryParams& QueryParams);

	UFUNCTION(BlueprintCallable, Category = "Collision", DisplayName = "Point Selector Query",
		meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore"))
	static bool BP_PointSelectorQuery(const UObject* WorldContextObject,
	                                  const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                                  UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                                  const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf,
	                                  TScriptInterface<IMSelectorQueryPointInterface>& OutLookAtPoint);

	UFUNCTION(BlueprintCallable, Category = "Collision", DisplayName = "Query Points",
		meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore"))
	static TArray<FMPointSelectorQueryScoredPointData> BP_QueryPoints(const UObject* WorldContextObject,
	                                                                  const FVector& ReferenceLocation,
	                                                                  const FVector& ReferenceDirection,
	                                                                  UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                                                                  const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf);
};

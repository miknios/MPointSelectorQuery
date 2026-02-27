// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MPointSelectorQuerySettingsAsset.generated.h"

class UMPointSelectorQueryFilter_Base;
class UMPointSelectorQueryFactor_Base;

USTRUCT(BlueprintType)
struct FMPointSelectorFactorConfigEntryData
{
	GENERATED_BODY()

	// How important is this factor in look at point calculation logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	UMPointSelectorQueryFactor_Base* Selector = nullptr;
};

UCLASS()
class MPOINTSELECTORQUERY_API UMPointSelectorQuerySettingsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMPointSelectorFactorConfigEntryData> SelectorFactorConfigs;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UMPointSelectorQueryFilter_Base>> Filters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceMax = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngleMax = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LookAtPointSelectionOverlapProfileName;
	
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	bool bDebug = false;
};


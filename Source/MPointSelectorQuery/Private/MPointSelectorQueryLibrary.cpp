// Copyright (c) Miknios. All rights reserved.


#include "MPointSelectorQueryLibrary.h"

#include "MDebug.h"
#include "MMath.h"
#include "MPointSelectorQueryFactor.h"
#include "MPointSelectorQueryFilter.h"
#include "MPointSelectorQueryModule.h"
#include "MPointSelectorQuerySettingsAsset.h"
#include "MSelectorQueryPoint.h"
#include "Engine/OverlapResult.h"

namespace
{
	FCollisionQueryParams MakeCollisionQueryParamsForBP(const UObject* WorldContextObject,
	                                                    const TArray<AActor*>& ActorsToIgnore,
	                                                    const bool bIgnoreSelf)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActors(ActorsToIgnore);
		Params.bTraceComplex = false;

		if (bIgnoreSelf)
		{
			const AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
			if (IgnoreActor)
			{
				Params.AddIgnoredActor(IgnoreActor);
			}
			else
			{
				// find owner
				const UObject* CurrentObject = WorldContextObject;
				while (CurrentObject)
				{
					CurrentObject = CurrentObject->GetOuter();
					IgnoreActor = Cast<AActor>(CurrentObject);
					if (IgnoreActor)
					{
						Params.AddIgnoredActor(IgnoreActor);
						break;
					}
				}
			}
		}

		return Params;
	}

	void OverlapCommon(const UWorld& World, const FVector& ReferenceLocation, const UMPointSelectorQuerySettingsAsset& SettingsAsset,
	                   const FCollisionQueryParams& QueryParams, TArray<FOverlapResult>& OutOverlapResults)
	{
		const FVector OverlapPos = ReferenceLocation;
		const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SettingsAsset.DistanceMax);

		World.OverlapMultiByProfile(OutOverlapResults, OverlapPos, FQuat::Identity,
		                            SettingsAsset.LookAtPointSelectionOverlapProfileName,
		                            CollisionShape, QueryParams);
	}

	bool CheckPointPassesFilters(const UObject* WorldContextObject, UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                             const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                             const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
	{
		// Distance and angle are not implemented with optional filters because they are used in filters and factors
		//		for stuff like max trace distance or for weighting points based on an angle to reference a direction
		const float Distance = FVector::Distance(ReferenceLocation, LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()));
		if (Distance > SettingsAsset->DistanceMax)
		{
			return false;
		}

		const FVector ToLookAtPointVector = MMath::FromToVector(ReferenceLocation,
		                                                        LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()));
		const float Angle = MMath::AngleBetweenVectorsDeg(ReferenceDirection, ToLookAtPointVector);
		if (Angle > SettingsAsset->AngleMax)
		{
			return false;
		}

		for (const UMPointSelectorQueryFilter_Base* QueryFilter : SettingsAsset->Filters)
		{
			if (QueryFilter == nullptr)
			{
				continue;
			}

			const bool FilterPassed = QueryFilter->FilterPoint(WorldContextObject,
			                                                   ReferenceLocation,
			                                                   ReferenceDirection,
			                                                   SettingsAsset,
			                                                   LookAtPoint);
			if (!FilterPassed)
			{
				return false;
			}
		}

		return true;
	}

	float CalculateScore(const UObject* WorldContextObject, UMPointSelectorQuerySettingsAsset* SettingsAsset,
	                     const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	                     const TScriptInterface<IMSelectorQueryPointInterface>& LookAtPoint)
	{
		float Score = 0;
		for (const FMPointSelectorFactorConfigEntryData& FactorConfig : SettingsAsset->SelectorFactorConfigs)
		{
			if (FactorConfig.Selector == nullptr)
			{
				M::Debug::LogUserError(LogPointSelectorQuery,
				                       TEXT("Can't apply selector factor to score because SelectorFactorConfig.Selector is null"),
				                       SettingsAsset);
				continue;
			}

			const float FactorValue = FactorConfig.Selector->CalculateFactorValueForLookAtPoint(
				WorldContextObject, ReferenceLocation, ReferenceDirection, SettingsAsset, LookAtPoint);
			Score += FactorValue * FactorConfig.Weight;
		}

		// TODO: remove this in favor of VLOGs? problem is that with VLOGs we can't filter specific query,
		//		so maybe we should still use bDebug and only then VLOG the points
		if (SettingsAsset->bDebug)
		{
			DrawDebugString(WorldContextObject->GetWorld(),
			                IMSelectorQueryPointInterface::Execute_GetLookAtLocation(LookAtPoint.GetObject()) + FVector(30, 30, 30),
			                FString::Printf(TEXT("%.2f"), Score),
			                nullptr,
			                FColor::Green,
			                0.f,
			                false,
			                3);
		}

		return Score;
	}
}

bool UMPointSelectorQueryLibrary::PointSelectorQuery(const UObject* WorldContextObject, const FVector& ReferenceLocation,
                                                     const FVector& ReferenceDirection, UMPointSelectorQuerySettingsAsset* SettingsAsset,
                                                     const FCollisionQueryParams& QueryParams,
                                                     TScriptInterface<IMSelectorQueryPointInterface>& OutLookAtPoint)
{
	if (WorldContextObject == nullptr)
	{
		M::Debug::LogUserError(LogPointSelectorQuery, TEXT("Can't query point because WorldContextObject is null"), WorldContextObject);
		return false;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
	{
		return false;
	}

	if (SettingsAsset == nullptr)
	{
		M::Debug::LogUserError(LogPointSelectorQuery, TEXT("Can't query point because SettingsAsset is null"), WorldContextObject);
		return false;
	}

	TArray<FOverlapResult> OverlapResults;
	OverlapCommon(*World, ReferenceLocation, *SettingsAsset, QueryParams, OverlapResults);

	float ScoreBest = -1;
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		UPrimitiveComponent* PrimitiveHit = OverlapResult.GetComponent();
		if (!PrimitiveHit->Implements<UMSelectorQueryPointInterface>())
		{
			continue;
		}

		TScriptInterface<IMSelectorQueryPointInterface> LookAtPoint = TScriptInterface<IMSelectorQueryPointInterface>(PrimitiveHit);

		const bool bPointPassedFilters = CheckPointPassesFilters(WorldContextObject, SettingsAsset, ReferenceLocation, ReferenceDirection,
		                                                         LookAtPoint);
		if (!bPointPassedFilters)
		{
			continue;
		}

		const float Score = CalculateScore(WorldContextObject, SettingsAsset, ReferenceLocation, ReferenceDirection, LookAtPoint);

		UE_VLOG_SPHERE(WorldContextObject, LogPointSelectorQuery, Display, LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()),
		               10.f, FColor::Green, TEXT("%.2f"), Score);

		if (Score > ScoreBest)
		{
			ScoreBest = Score;
			OutLookAtPoint = MoveTemp(LookAtPoint);
		}
	}

	return ScoreBest > -1;
}

TArray<FMPointSelectorQueryScoredPointData> UMPointSelectorQueryLibrary::QueryPoints(const UObject* WorldContextObject,
                                                                                     const FVector& ReferenceLocation,
                                                                                     const FVector& ReferenceDirection,
                                                                                     UMPointSelectorQuerySettingsAsset* SettingsAsset,
                                                                                     const FCollisionQueryParams& QueryParams)
{
	TArray<FMPointSelectorQueryScoredPointData> Result;

	if (WorldContextObject == nullptr)
	{
		M::Debug::LogUserError(LogPointSelectorQuery, TEXT("Can't query point because WorldContextObject is null"), WorldContextObject);
		return MoveTemp(Result);
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
	{
		return MoveTemp(Result);
	}

	if (SettingsAsset == nullptr)
	{
		M::Debug::LogUserError(LogPointSelectorQuery, TEXT("Can't query point because SettingsAsset is null"), WorldContextObject);
		return MoveTemp(Result);
	}

	TArray<FOverlapResult> OverlapResults;
	OverlapCommon(*World, ReferenceLocation, *SettingsAsset, QueryParams, OverlapResults);

	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		UPrimitiveComponent* PrimitiveHit = OverlapResult.GetComponent();
		if (!PrimitiveHit->Implements<UMSelectorQueryPointInterface>())
		{
			continue;
		}

		TScriptInterface<IMSelectorQueryPointInterface> LookAtPoint = TScriptInterface<IMSelectorQueryPointInterface>(PrimitiveHit);

		const bool bPointPassedFilters = CheckPointPassesFilters(WorldContextObject, SettingsAsset, ReferenceLocation, ReferenceDirection,
		                                                         LookAtPoint);
		if (!bPointPassedFilters)
		{
			continue;
		}

		const float Score = CalculateScore(WorldContextObject, SettingsAsset, ReferenceLocation, ReferenceDirection, LookAtPoint);

		Result.Emplace(LookAtPoint, Score);

		UE_VLOG_SPHERE(WorldContextObject, LogPointSelectorQuery, Display, LookAtPoint->Execute_GetLookAtLocation(LookAtPoint.GetObject()),
		               10.f, FColor::Green, TEXT("%.2f"), Score);
	}

	// Sort from highest to lowest
	Result.Sort([](const FMPointSelectorQueryScoredPointData& A, const FMPointSelectorQueryScoredPointData& B)
	{
		return A.Score > B.Score;
	});

	return MoveTemp(Result);
}

bool UMPointSelectorQueryLibrary::BP_PointSelectorQuery(
	const UObject* WorldContextObject,
	const FVector& ReferenceLocation, const FVector& ReferenceDirection,
	UMPointSelectorQuerySettingsAsset* SettingsAsset,
	const TArray<AActor*>& ActorsToIgnore, const bool bIgnoreSelf,
	TScriptInterface<IMSelectorQueryPointInterface>& OutLookAtPoint)
{
	const FCollisionQueryParams Params = MakeCollisionQueryParamsForBP(WorldContextObject, ActorsToIgnore, bIgnoreSelf);
	const bool Result = PointSelectorQuery(WorldContextObject, ReferenceLocation, ReferenceDirection, SettingsAsset, Params,
	                                       OutLookAtPoint);
	return Result;
}

TArray<FMPointSelectorQueryScoredPointData> UMPointSelectorQueryLibrary::BP_QueryPoints(const UObject* WorldContextObject,
                                                                                        const FVector& ReferenceLocation,
                                                                                        const FVector& ReferenceDirection,
                                                                                        UMPointSelectorQuerySettingsAsset* SettingsAsset,
                                                                                        const TArray<AActor*>& ActorsToIgnore,
                                                                                        bool bIgnoreSelf)
{
	const FCollisionQueryParams Params = MakeCollisionQueryParamsForBP(WorldContextObject, ActorsToIgnore, bIgnoreSelf);
	return QueryPoints(WorldContextObject, ReferenceLocation, ReferenceDirection, SettingsAsset, Params);
}

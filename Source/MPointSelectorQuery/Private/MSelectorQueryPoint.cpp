// Copyright (c) Miknios. All rights reserved.

#include "MSelectorQueryPoint.h"

FVector UMSelectorQueryPoint_GenericSphere::GetLookAtLocation_Implementation() const
{
	return GetComponentLocation();
}

float UMSelectorQueryPoint_GenericSphere::GetImportanceValue_Implementation() const
{
	if (ImportanceValueAsset != nullptr)
	{
		return ImportanceValueAsset->ImportanceValue;
	}

	return ImportanceValue;
}

AActor* UMSelectorQueryPoint_GenericSphere::GetActorOwner_Implementation() const
{
	return GetOwner();
}

void UMSelectorQueryPoint_GenericSphere::SetImportanceValue(const float NewImportanceValue)
{
	ImportanceValue = NewImportanceValue;
}


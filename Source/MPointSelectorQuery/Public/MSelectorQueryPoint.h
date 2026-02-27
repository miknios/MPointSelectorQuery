// Copyright (c) Miknios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "MSelectorQueryPoint.generated.h"

UINTERFACE()
class UMSelectorQueryPointInterface : public UInterface
{
	GENERATED_BODY()
};

class MPOINTSELECTORQUERY_API IMSelectorQueryPointInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetLookAtLocation() const;

	// Returns value between 0-1 that informs how important is that specific look at point in comparison to others
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetImportanceValue() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetActorOwner() const;
};

UCLASS()
class UMSelectorQueryPoint_ImportanceValueAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float ImportanceValue = 1;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMSelectorQueryPoint_GenericSphere : public USphereComponent, public IMSelectorQueryPointInterface
{
	GENERATED_BODY()

public:
	// IMSelectorQueryPointInterface
	virtual FVector GetLookAtLocation_Implementation() const override;
	virtual float GetImportanceValue_Implementation() const override;
	virtual AActor* GetActorOwner_Implementation() const override;
	// ~ IMSelectorQueryPointInterface

	UFUNCTION(BlueprintCallable)
	void SetImportanceValue(const float NewImportanceValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMSelectorQueryPoint_ImportanceValueAsset> ImportanceValueAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1", EditCondition = "ImportanceValueAsset == nullptr"))
	float ImportanceValue = 1;
};


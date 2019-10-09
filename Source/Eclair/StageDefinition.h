// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StageType.h"
#include "Engine/Texture2D.h"
#include "StageDefinition.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ECLAIR_API UStageDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UStageDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Slug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EStageType StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255", UIMin = "0", UIMax = "64"))
		int ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DisplayTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SubTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255", UIMin = "0", UIMax = "64"))
		int ItemCountToUnlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
};
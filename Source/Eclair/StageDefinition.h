// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString Slug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString LevelName;
	
};

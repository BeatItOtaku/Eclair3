// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EclairConversationItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEclairConversationItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString content;

	FEclairConversationItem()
	{
		content = "";
	}
	
};

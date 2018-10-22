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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		uint8 character;

	/* キャラクターの表情差分 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		uint8 variation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		uint8 name_overwrite;

	FEclairConversationItem()
	{
		content = "Content";
		character = 0;
		variation = 0;
	}

	FEclairConversationItem(FString content, uint8 character, uint8 valiation) {
		this->content = content;
		this->character = character;
		this->variation = valiation;
	}
	
};

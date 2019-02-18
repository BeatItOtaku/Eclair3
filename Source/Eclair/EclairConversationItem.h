// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "EclairConversationItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ECLAIR_API FEclairConversationItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		TArray<int> Character;

	/* キャラクターの表情差分 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		int Variation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString NameOverwrite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		float Duration;

	FEclairConversationItem()
	{
		Content = "Content";
		Character = {0};
		Variation = 0;
		NameOverwrite = "";
		Duration = 3;
	}

	FEclairConversationItem(FString content, TArray<int> character, int valiation, FString nameOverwrite, float duration) {
		this->Content = content;
		this->Character = character;
		this->Variation = valiation;
		this->NameOverwrite = nameOverwrite;
		this->Duration = duration;
	}
};


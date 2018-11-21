// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal-UE4.hxx"
#include <sstream>

#include "EclairConversationItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEclairConversationItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		TArray<uint8> Character;

	/* キャラクターの表情差分 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		uint8 Variation;

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

	FEclairConversationItem(FString content, TArray<uint8> character, uint8 valiation, FString nameOverwrite, uint8 duration) {
		this->Content = content;
		this->Character = character;
		this->Variation = valiation;
		this->NameOverwrite = nameOverwrite;
		this->Duration = duration;
	}
};

template <typename A>
void serialize(A& a, FEclairConversationItem& e)
{
	UE_LOG(LogTemp, Log, TEXT("ahgoe"));
	a
	(cereal::make_nvp("content", e.Content)
		, cereal::make_nvp("character", e.Character)
		//, cereal::make_nvp("variation", e.Variation)
		, cereal::make_nvp("name-overwrite", e.NameOverwrite)
		, cereal::make_nvp("duration", e.Duration)
	);
}

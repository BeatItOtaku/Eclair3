// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EclairConversationItem.h"
#include "EclairConversationType.h"

#if WITH_EDITORONLY_DATA
#include "cereal/cereal.hpp"
#include "cereal-UE4.hxx"
#include <sstream>
#endif

#include "EclairConversation.generated.h"

/**
 * 
 */
UCLASS()
class ECLAIR_API UEclairConversation : public UObject
{
	GENERATED_BODY()
	
public:
	UEclairConversation();

	UPROPERTY(EditAnywhere)
		FString Description;

	UPROPERTY(EditAnywhere)
		EConversationType Type;

	UPROPERTY(EditAnywhere)
		TArray<uint8> CharacterLeft;

	UPROPERTY(EditAnywhere)
		TArray<uint8> CharacterRight;

	UPROPERTY(EditAnywhere)
		TArray<FEclairConversationItem> Items;

#if WITH_EDITORONLY_DATA

	template<typename A>
	void serialize(A& a) {
		UE_LOG(LogTemp, Log, TEXT("conversation"));
		a
		( cereal::make_nvp("description", Description),
			cereal::make_nvp("type", Type),
			cereal::make_nvp("character-left", CharacterLeft),
			cereal::make_nvp("character-right", CharacterRight),
			cereal::make_nvp("content", Items)
		);

	}
	
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
		class UAssetImportData* AssetImportData;
#endif

};
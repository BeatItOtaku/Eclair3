// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EclairConversationItem.h"
#include "EclairConversationType.h"
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

	template<typename A>
	void serialize(A& a, UEclairConversation& in) {
		a
		( cereal::make_nvp("description"), in.Description),
			cereal::make_nvp("type", in.Type),
			cereal::make_nvp("character-left", in.CharacterLeft),
			cereal::make_nvp("character-right", in.CharacterRight)
		);

	}
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
		class UAssetImportData* AssetImportData;
#endif

};
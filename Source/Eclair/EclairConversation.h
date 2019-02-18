// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EclairConversationItem.h"
#include "EclairConversationType.h"
#include "ConversationCharacter.h"
#include "Styling/SlateBrush.h"

#include "EclairConversation.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ECLAIR_API UEclairConversation : public UObject
{
	GENERATED_BODY()
	
public:
	UEclairConversation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EConversationType Type;

	UPROPERTY(EditAnywhere)
		TArray<int> CharacterLeft;

	UPROPERTY(EditAnywhere)
		TArray<int> CharacterRight;

	UPROPERTY(EditAnywhere)
		TArray<FEclairConversationItem> Items;

	UFUNCTION(BlueprintCallable, Category = "EclairConversation")
		FEclairConversationItem Get(int index);

	UFUNCTION(BlueprintCallable, Category = "EclairConversation", BlueprintPure, meta = (CompactNodeTitle = "LENGTH"))
		int Length();

	UFUNCTION(BlueprintCallable, Category = "EclairConversation", BlueprintPure)
		static UConversationCharacter* GetCharacter(int index);

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
		class UAssetImportData* AssetImportData;
#endif

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Styling/SlateBrush.h"

#include "ConversationCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ECLAIR_API UConversationCharacter : public UObject
{
	GENERATED_BODY()
	
public:
	UConversationCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		TArray<FSlateBrush> Brushes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EclairConversation")
		FColor Color;
};

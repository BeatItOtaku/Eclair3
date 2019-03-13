// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConversationCharacter.h"

#include "EclairConversationSetting.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class ECLAIR_API UEclairConversationSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = EclairConversationSetting, meta = (AllowedClasses = "ConversationCharacter"))
		TArray<FSoftObjectPath> Characters;

	//UEclairConversationSetting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer);
};
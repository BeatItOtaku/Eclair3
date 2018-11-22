// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EclairConversationSetting.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class ECLAIRED_API UEclairConversationSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = EclairConversationSetting)
		bool Check0;

	//UEclairConversationSetting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer);
};
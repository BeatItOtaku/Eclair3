// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
		int32 ValueA;

	UPROPERTY(EditAnywhere)
		int32 ValueB;

	UPROPERTY(EditAnywhere)
		int32 ValueC;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
		class UAssetImportData* AssetImportData;
#endif

};

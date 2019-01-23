// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Json.h"
#include "AES.h"
#include "Base64.h"
#include "ResultEncryptor.generated.h"

/**
 * 
 */
UCLASS()
class ECLAIR_API UResultEncryptor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Encrypt")
	static FString EncryptResult(int time, int item, FDateTime timestamp);

	static FString Encrypt(FString str);
};

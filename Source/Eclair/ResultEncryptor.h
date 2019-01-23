// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Json.h"

#include "AES.h"
#include "Base64.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#if PLATFORM_WINDOWS
#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/opensslconf.h"
#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/aes.h"
#elif PLATFORM_MAC
#include "ThirdParty/OpenSSL/1.0.2g/include/Mac/openssl/aes.h"
#endif
THIRD_PARTY_INCLUDES_END
#undef UI

#define AES_KEYLENGTH 256

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

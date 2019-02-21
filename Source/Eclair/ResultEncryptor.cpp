// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultEncryptor.h"

FString UResultEncryptor::EncryptResult(FString stage, FString stageName, int time, int item, FDateTime timestamp, FString version) {
    
#if PLATFORM_WINDOWS

	//JSONを生成
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("stage", stage);
	//JsonObject->SetStringField("stage-name", stageName);
	JsonObject->SetStringField("time", FString::FromInt(time));
	JsonObject->SetStringField("item", FString::FromInt(item));
	JsonObject->SetStringField("timestamp", timestamp.ToIso8601());
	JsonObject->SetStringField("version", version);

	// Content文字列にJSONフォーマットで書き出し
	FString Content;
	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject, Writer);

	return Encrypt(Content);
    
#else
    return "";
#endif
}

bool UResultEncryptor::isEncryptSupported()
{
#if PLATFORM_WINDOWS
	return true;
#else
	return false;
#endif
}

FString UResultEncryptor::Encrypt(FString str)
{
#if PLATFORM_WINDOWS
    
	if (str.IsEmpty()) return str;  //empty string? do nothing

	//Size設定 ブロックサイズ単位になるようにパディングをとる
	uint32 Size;
	Size = str.Len();
	Size = Size + (AES_BLOCK_SIZE - (Size % AES_BLOCK_SIZE));

	// キー KeyChar を生成
	FString Key = "eclairhtl2017thethunderboltX8kr2";
	TCHAR *KeyTChar = Key.GetCharArray().GetData();
	unsigned char *KeyChar = new unsigned char[AES_KEYLENGTH];
	memset(KeyChar, 0x00, AES_KEYLENGTH);
	strcpy((char*)KeyChar, (char*)TCHAR_TO_UTF8(KeyTChar));

	//strをもとに暗号化するバイト列 in を用意
	TCHAR *c = str.GetCharArray().GetData();
	unsigned char *in = new unsigned char[Size];
	memset(in, 0x00, Size);
	strcpy((char*)in, (char*)TCHAR_TO_UTF8(c));
	//PKCS#7パディングを付加
	int padSize = Size - str.Len();
	memset(in + str.Len(), padSize, padSize);

	//暗号化されたバイト列が入る out
	unsigned char* out = new unsigned char[Size];

	//初期化ベクター
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);

	//DebugMessage
	//FString HexBlobStr = FString::FromHexBlob(in, Size);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Size:%d, strByte:%s, Blob:%s"), Size, UTF8_TO_TCHAR(in), HexBlobStr.GetCharArray().GetData()));

	//暗号化
	if (in != nullptr) {
		AES_KEY aesKey;
		AES_set_encrypt_key((unsigned char*)KeyChar, AES_KEYLENGTH, &aesKey);
		AES_cbc_encrypt((unsigned char*)in, out,Size, &aesKey,iv, AES_ENCRYPT);
		str = FBase64::Encode(out, Size);
		str = FGenericPlatformHttp::UrlEncode(str);
		return str;
	}
	return ""; //If failed return empty string
#else
    return "";
#endif
}

FString UResultEncryptor::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

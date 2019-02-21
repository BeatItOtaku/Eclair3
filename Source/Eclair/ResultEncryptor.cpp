// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultEncryptor.h"

FString UResultEncryptor::EncryptResult(FString stage, FString stageName, int time, int item, FDateTime timestamp, FString version) {
    
#if PLATFORM_WINDOWS

	//JSON�𐶐�
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("stage", stage);
	//JsonObject->SetStringField("stage-name", stageName);
	JsonObject->SetStringField("time", FString::FromInt(time));
	JsonObject->SetStringField("item", FString::FromInt(item));
	JsonObject->SetStringField("timestamp", timestamp.ToIso8601());
	JsonObject->SetStringField("version", version);

	// Content�������JSON�t�H�[�}�b�g�ŏ����o��
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

	//Size�ݒ� �u���b�N�T�C�Y�P�ʂɂȂ�悤�Ƀp�f�B���O���Ƃ�
	uint32 Size;
	Size = str.Len();
	Size = Size + (AES_BLOCK_SIZE - (Size % AES_BLOCK_SIZE));

	// �L�[ KeyChar �𐶐�
	FString Key = "eclairhtl2017thethunderboltX8kr2";
	TCHAR *KeyTChar = Key.GetCharArray().GetData();
	unsigned char *KeyChar = new unsigned char[AES_KEYLENGTH];
	memset(KeyChar, 0x00, AES_KEYLENGTH);
	strcpy((char*)KeyChar, (char*)TCHAR_TO_UTF8(KeyTChar));

	//str�����ƂɈÍ�������o�C�g�� in ��p��
	TCHAR *c = str.GetCharArray().GetData();
	unsigned char *in = new unsigned char[Size];
	memset(in, 0x00, Size);
	strcpy((char*)in, (char*)TCHAR_TO_UTF8(c));
	//PKCS#7�p�f�B���O��t��
	int padSize = Size - str.Len();
	memset(in + str.Len(), padSize, padSize);

	//�Í������ꂽ�o�C�g�񂪓��� out
	unsigned char* out = new unsigned char[Size];

	//�������x�N�^�[
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);

	//DebugMessage
	//FString HexBlobStr = FString::FromHexBlob(in, Size);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Size:%d, strByte:%s, Blob:%s"), Size, UTF8_TO_TCHAR(in), HexBlobStr.GetCharArray().GetData()));

	//�Í���
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

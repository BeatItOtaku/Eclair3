// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultEncryptor.h"

FString UResultEncryptor::EncryptResult(int time, int item, FDateTime timestamp) {

	//JSON�𐶐�
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("time", FString::FromInt(time));
	JsonObject->SetStringField("item", FString::FromInt(item));
	JsonObject->SetStringField("timestamp", timestamp.ToIso8601());

	// Content�������JSON�t�H�[�}�b�g�ŏ����o��
	FString Content;
	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject, Writer);

	return Encrypt("{\"time\":86,\"item\":12,\"timestamp\":1548231246}");
	//return Encrypt(Content);
}

FString UResultEncryptor::Encrypt(FString str)
{
	if (str.IsEmpty()) return str;  //empty string? do nothing

	uint8* Blob; //we declere uint8 pointer
	uint32 Size; //for size calculation

	//first we need to calculate the size of array, encrypted data will be processed in blocks so
	//data size need to be aligned with block size

	Size = str.Len();
	Size = Size + (FAES::AESBlockSize - (Size % FAES::AESBlockSize));

	// �L�[�𐶐�
	FString Key = "hogehogefooofooohogehogefooofooo";                            // Choose a key then...
	TCHAR *KeyTChar = Key.GetCharArray().GetData();            // ...turn key string...
	ANSICHAR *KeyAnsi = (ANSICHAR*)TCHAR_TO_ANSI(KeyTChar);

	Blob = new uint8[Size]; //So once we calculated size we allocating space in memory 
								//which we use for encryption

	TCHAR *c = str.GetCharArray().GetData();
	unsigned char *strByte = (unsigned char*)TCHAR_TO_UTF8(c);

	int utf8Size = 256; //strlen((char*)strByte);
	unsigned char* out = new unsigned char[utf8Size];

	UE_LOG(LogTemp, Verbose, TEXT("Size:%d, uft8Size:%d"), Size, utf8Size);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Size:%d, uft8Size:%d, strByte:%s"), Size, utf8Size, UTF8_TO_TCHAR(strByte)));

								//We filling allocated space with string to process
	if (strByte != nullptr) {
		//FAES::EncryptData((unsigned char*)c, Size, KeyAnsi); //We encrypt the data, don't know how you want to input key
		AES_KEY aesKey;
		AES_set_encrypt_key((unsigned char*)KeyAnsi, AES_KEYLENGTH, &aesKey);
		AES_ecb_encrypt((unsigned char*)strByte, out, &aesKey, AES_ENCRYPT);
		str = FBase64::Encode(out, utf8Size);
		delete Blob; //deleting allocation for safety
		return str; //and return it
	}
	delete Blob; //deleting allocation for safety
	return ""; //If failed return empty string
}

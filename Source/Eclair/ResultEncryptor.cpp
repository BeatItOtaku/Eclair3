// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultEncryptor.h"

FString UResultEncryptor::EncryptResult(int time, int item, FDateTime timestamp) {

	//JSONを生成
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("time", FString::FromInt(time));
	JsonObject->SetStringField("item", FString::FromInt(item));
	JsonObject->SetStringField("timestamp", timestamp.ToIso8601());

	// Content文字列にJSONフォーマットで書き出し
	FString Content;
	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject, Writer);

	return Encrypt(Content);
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

	// キーを生成
	FString Key = "hogehogefooofooohogehogefooofooo";                            // Choose a key then...
	TCHAR *KeyTChar = Key.GetCharArray().GetData();            // ...turn key string...
	ANSICHAR *KeyAnsi = (ANSICHAR*)TCHAR_TO_ANSI(KeyTChar);

	Blob = new uint8[Size]; //So once we calculated size we allocating space in memory 
								//which we use for encryption

								//We filling allocated space with string to process
	if (FString::ToBlob(str, Blob, str.Len())) {
		FAES::EncryptData(Blob, Size, KeyAnsi); //We encrypt the data, don't know how you want to input key
		str = FString::FromHexBlob(Blob, Size); //now generate hex string of encrypted data
		delete Blob; //deleting allocation for safety
		return str; //and return it
	}
	delete Blob; //deleting allocation for safety
	return ""; //If failed return empty string
}

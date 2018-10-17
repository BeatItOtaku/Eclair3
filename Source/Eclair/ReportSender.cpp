// Fill out your copyright notice in the Description page of Project Settings.

#include "ReportSender.h"


// Sets default values
AReportSender::AReportSender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void AReportSender::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReportSender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReportSender::AddRequestParameter(const FString& Key, const FString& Value)
{
	RequestParameters.Add(Key, Value);
}

void AReportSender::RemoveRequestParameter(const FString& Key)
{
	RequestParameters.Remove(Key);
}

/*Http call*/
void AReportSender::SendReport(FString URL)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AReportSender::OnResponse);
	//This is the url on which to process the request
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	//PCスペックを取得
	RequestParameters.Add("platform", UGameplayStatics::GetPlatformName());
	RequestParameters.Add("cpu", FWindowsPlatformMisc::GetCPUBrand());
	RequestParameters.Add("gpu", FWindowsPlatformMisc::GetPrimaryGPUBrand());

	//JSONを生成
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	for (const auto& Entry : RequestParameters)
	{
		JsonObject->SetStringField(*Entry.Key, *Entry.Value);
	}

	// Content文字列にJSONフォーマットで書き出し
	FString Content;
	auto Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject, Writer);

	Request->SetContentAsString(Content);

	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void AReportSender::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	/*
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		int32 recievedInt = JsonObject->GetIntegerField("customInt");

		//Output it to the engine
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	}*/
	OnResponseReceived.Broadcast();
}
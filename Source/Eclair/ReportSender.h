// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Engine.h"
#include "ReportSender.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReportSenderOnResponseReceived);

UCLASS()
class ECLAIR_API AReportSender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReportSender();
	FHttpModule* Http;

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category="Http")
	void SendReport(FString URL);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable, Category="Http")
	FReportSenderOnResponseReceived OnResponseReceived;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Http")
	void AddRequestParameter(const FString & Key, const FString & Value);

	UFUNCTION(BlueprintCallable, Category = "Http")
	void RemoveRequestParameter(const FString & Key);

private:
	TMap<FString, FString> RequestParameters;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "StageDefinitionFactory.generated.h"

/**
 * 
 */
UCLASS()
class ECLAIRED_API UStageDefinitionFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UStageDefinitionFactory();

	virtual bool DoesSupportClass(UClass* Class) override;
	virtual UClass* ResolveSupportedClass() override;

	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn
	) override;
	
};

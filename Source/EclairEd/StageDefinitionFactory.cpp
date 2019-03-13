// Fill out your copyright notice in the Description page of Project Settings.

#include "StageDefinitionFactory.h"
#include "StageDefinition.h"

UStageDefinitionFactory::UStageDefinitionFactory() : Super() {
	SupportedClass = UStageDefinition::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
	bText = true;
}

bool UStageDefinitionFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UStageDefinition::StaticClass());
}
UClass* UStageDefinitionFactory::ResolveSupportedClass()
{
	return UStageDefinition::StaticClass();
}
UObject* UStageDefinitionFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UStageDefinition* NewStageDefinition = NewObject<UStageDefinition>(InParent, InClass, InName, Flags);
	return NewStageDefinition;
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "ConversationCharacterFactory.h"
#include "ConversationCharacter.h"

UConversationCharacterFactory::UConversationCharacterFactory() : Super() {
	SupportedClass = UConversationCharacter::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
	bText = true;
}

bool UConversationCharacterFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UConversationCharacter::StaticClass());
}
UClass* UConversationCharacterFactory::ResolveSupportedClass()
{
	return UConversationCharacter::StaticClass();
}
UObject* UConversationCharacterFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UConversationCharacter* NewConversationCharacter = NewObject<UConversationCharacter>(InParent, InClass, InName, Flags);
	return NewConversationCharacter;
}

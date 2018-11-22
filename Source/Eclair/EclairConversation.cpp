// Fill out your copyright notice in the Description page of Project Settings.

#include "EclairConversation.h"
#include "EclairConversationSetting.h"
#include "Eclair.h"

UEclairConversation::UEclairConversation()
	: Super()
{
	
}

FEclairConversationItem UEclairConversation::Get(int index)
{
	return Items[index];
}

uint8 UEclairConversation::Length()
{
	return Items.Num();
}

UConversationCharacter* UEclairConversation::GetCharacter(uint8 index)
{
	const UEclairConversationSetting* setting = GetDefault<UEclairConversationSetting>();
	TArray<FSoftObjectPath> path = setting->Characters;
	UConversationCharacter* c = Cast<UConversationCharacter>(setting->Characters[index].TryLoad());
	return c;
}

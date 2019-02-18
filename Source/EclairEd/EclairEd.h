// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EclairConversation.h"
#include "EclairConversationItem.h"
#include "cereal/cereal.hpp"
#include "cereal-UE4.hxx"
#include <sstream>

template<typename A>
void serialize(A& a,UEclairConversation & c) {
    UE_LOG(LogTemp, Log, TEXT("conversation"));
    a
    ( cereal::make_nvp("description", c.Description),
     cereal::make_nvp("type", c.Type),
     cereal::make_nvp("character-left", c.CharacterLeft),
     cereal::make_nvp("character-right", c.CharacterRight),
     cereal::make_nvp("content", c.Items)
     );
    
}

template <typename A>
void serialize(A& a, FEclairConversationItem& e)
{
    UE_LOG(LogTemp, Log, TEXT("ahgoe"));
    a
    (cereal::make_nvp("content", e.Content)
     , cereal::make_nvp("character", e.Character)
     //, cereal::make_nvp("variation", e.Variation)
     , cereal::make_nvp("name-overwrite", e.NameOverwrite)
     , cereal::make_nvp("duration", e.Duration)
     );
}

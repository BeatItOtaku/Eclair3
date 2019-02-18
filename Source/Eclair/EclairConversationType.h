// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EConversationType : uint8
{
	CT_Auto	UMETA(DisplayName = "Automatic"),
	CT_Manual	UMETA(DisplayName = "Manual"),
	CT_Event	UMETA(DisplayName = "Driven by Events"),
	Num	UMETA(Hidden)
};

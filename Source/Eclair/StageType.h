#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EStageType : uint8
{
	ST_Main	UMETA(DisplayName = "Main"),
	CT_Challenge	UMETA(DisplayName = "Challenge"),
	Num	UMETA(Hidden)
};
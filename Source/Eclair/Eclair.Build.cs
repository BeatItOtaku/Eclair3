// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class Eclair : ModuleRules
{
	public Eclair(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Http", "Json", "SlateCore"});

        var base_path = Path.GetDirectoryName(RulesCompiler.GetFileNameFromType(GetType()));
        string third_party_path = Path.Combine(base_path, "..", "..", "Thirdparty");
        PublicIncludePaths.Add(Path.Combine(third_party_path, "cereal", "include"));
        PublicIncludePaths.Add(Path.Combine(third_party_path, "cereal-UE4", "include"));
    }
}

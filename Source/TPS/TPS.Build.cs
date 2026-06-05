// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;

public class TPS : ModuleRules
{
    public TPS(ReadOnlyTargetRules Target) : base(Target)
    {
        Console.WriteLine(
            "Unreal Version: {0}.{1}.{2}", Target.Version.MajorVersion, Target.Version.MinorVersion, Target.Version.PatchVersion);
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Json", "JsonUtilities" });
        PublicIncludePaths.Add("TPS");
    }
}

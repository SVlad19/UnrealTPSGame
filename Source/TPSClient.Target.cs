// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.Collections.Generic;
public class TPSClientTarget : TargetRules
{
    public TPSClientTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Client;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        ExtraModuleNames.AddRange(new string[] { "TPS" });
    }
}

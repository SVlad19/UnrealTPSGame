// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;
using EpicGames.Core;

public class TPSTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;
    public TPSTarget(TargetInfo Target) : base(Target)
    {
        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }

        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("TPS");
    }
}

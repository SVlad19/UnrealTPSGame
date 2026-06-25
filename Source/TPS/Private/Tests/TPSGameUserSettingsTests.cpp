// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/TPSGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Settings/TPSGameUserSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExist, "TPSGame.GameUserSettings.SettingsShouldExist",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

bool FSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    TestTrueExpr(UTPSGameUserSettings::Get() != nullptr);
    TestTrueExpr(UTPSGameUserSettings::Get()->GetVideoSettings().Num() == 7);

    return true;
}

#endif

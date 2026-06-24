// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Character/TPSPlayerController.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/TPSGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Settings/TPSGameSetting.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePause, "TPSGame.UI.PauseMenuShouldBeVisibleOnGamePause",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldCollapsedOnGameUnPause, "TPSGame.UI.PauseMenuShouldCollapsedOnGameUnPause",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVIdeoSettingsExist, "TPSGame.UI.AllVIdeoSettingsExist",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingCanBeApplied, "TPSGame.UI.SettingCanBeApplied",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldWork, "TPSGame.UI.AutoBenchmarkShouldWork",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

using namespace TPS::Test;

namespace
{

void NextSettingClick(int32 SettingIndex)
{
    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const auto SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
    const auto NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
    NextSettingButton->OnClicked.Broadcast();
}

void DoBenchmarkClick()
{
    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    BenchmarkButton->OnClicked.Broadcast();
}

}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePause::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrueExpr(PC != nullptr);

    UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();

    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            TestTrue(TEXT("Pause input was injected"), SimulateInput(PC, PC->PauseAction, FInputActionValue(true)));
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
            TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);
            return true;
        }));

    return true;
}

bool FPauseMenuShouldCollapsedOnGameUnPause::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrueExpr(PC != nullptr);

    UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();

    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            TestTrue(TEXT("Pause input was injected"), SimulateInput(PC, PC->PauseAction, FInputActionValue(true)));
            TestTrue(TEXT("Pause input was injected"), SimulateInput(PC, PC->PauseAction, FInputActionValue(true)));
            return true;
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
            TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
            return true;
        }));
    return true;
}

bool FAllVIdeoSettingsExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrueExpr(PC != nullptr);

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BenchmarkButton != nullptr);

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VerticalBox != nullptr);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSettings.Num());
    TestTrueExpr(VerticalBox->GetChildrenCount() > 0);

    return true;
}

bool FSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrue(TEXT("Pause input was injected"), SimulateInput(PC, PC->PauseAction, FInputActionValue(true)));

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    const int32 GIQualityBefore = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore == VideoSettings[2]->GetCurrentOption().Value);

    NextSettingClick(2);

    const int32 GIQualityAfter = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore != GIQualityAfter);
    TestTrueExpr(GIQualityAfter == VideoSettings[2]->GetCurrentOption().Value);

    UTPSGameUserSettings::Get()->SetGlobalIlluminationQuality(GIQualityBefore);
    UTPSGameUserSettings::Get()->ApplySettings(false);
    return true;
}

bool FAutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrue(TEXT("Pause input was injected"), SimulateInput(PC, PC->PauseAction, FInputActionValue(true)));

    DoBenchmarkClick();

    TArray<int32> SavedSettingsValue;
    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings)
    {
        SavedSettingsValue.Add(Setting->GetCurrentOption().Value);
    }

    // Random clicks

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        NextSettingClick(i);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingsValue[i] != VideoSettings[i]->GetCurrentOption().Value);
    }

    DoBenchmarkClick();

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingsValue[i] == VideoSettings[i]->GetCurrentOption().Value);
    }

    return true;
}
#endif

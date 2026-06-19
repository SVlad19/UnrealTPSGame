// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/TPSScreenshots.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Camera/CameraActor.h"
#include "Engine/DamageEvents.h"
#include "Character/TPSPlayerController.h"
#include "EnhancedInputSubsystems.h"

using namespace TPS::Test;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "TPSGame.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHUDShouldBeRendered, "TPSGame.Screenshots.MainPlayerHUDShouldBeRendered",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
    "TPSGame.Screenshots.HealthWidgetShouldBeRenderedCorrectlyAfterDamage",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetShouldBeRenderedWhenGamePaused,
    "TPSGame.Screenshots.SettingsWidgetShouldBeRenderedWhenGamePaused",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }
    const FTransform Transform{FVector{2490.000000, 730.000000, 160.000000}};
    ACameraActor* CameraActor = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera exists", CameraActor))
    {
        return false;
    }

    APlayerController* PC = World->GetFirstPlayerController();

    if (!TestNotNull("Player controller exists", PC))
    {
        return false;
    }

    PC->SetViewTarget(CameraActor);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));

    return true;
}

bool FMainPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }
    const FTransform Transform{FVector{2490.000000, 730.000000, 160.000000}};
    ACameraActor* CameraActor = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera exists", CameraActor))
    {
        return false;
    }

    APlayerController* PC = World->GetFirstPlayerController();

    if (!TestNotNull("Player controller exists", PC))
    {
        return false;
    }

    PC->SetViewTarget(CameraActor);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("main_player_HUD_screenshot"));

    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    if (!TestNotNull("Player controller exists", PC))
    {
        return false;
    }
    if (!TestNotNull("Player exists", PC->GetPawn().Get()))
    {
        return false;
    }

    const float DamageAmount = 40.f;
    PC->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));

    return true;
}

bool FSettingsWidgetShouldBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    if (!TestNotNull("Player controller exists", PC))
    {
        return false;
    }
    if (!TestNotNull("Player exists", PC->GetPawn().Get()))
    {
        return false;
    }

    SimulateInput(PC, PC->PauseAction, FInputActionValue(true));

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_screenshot"));

    return true;
}

#endif

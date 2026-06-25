// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/TPSPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Character/TPSPlayerController.h"
#include "Tests/TestUtils.h"
#include "EnhancedInputSubsystems.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerController.GameCanBePaused",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerController.GameCanBeUnPaused",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            SimulateInput(PC, PC->PauseAction, FInputActionValue(true));
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            TestTrueExpr(PC->IsPaused());
            TestTrueExpr(PC->bShowMouseCursor);
            return true;
        }));

    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            SimulateInput(PC, PC->PauseAction, FInputActionValue(true));
            return true;
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            SimulateInput(PC, PC->PauseAction, FInputActionValue(true));
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetTestGameWorld()->GetFirstPlayerController());
            TestTrueExpr(!PC->IsPaused());
            TestTrueExpr(!PC->bShowMouseCursor);
            return true;
        }));

    // SimulateInput(PC, PC->PauseAction, FInputActionValue(true));
    // SimulateInput(PC, PC->PauseAction, FInputActionValue(true));
    // TestTrueExpr(!PC->IsPaused());
    // TestTrueExpr(!PC->bShowMouseCursor);

    return true;
}

#endif

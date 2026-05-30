// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Items/TPSInventoryItem.h"
#include "TPSCharacter.h"
#include "InputActionValue.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(
    FJumpLatentCommand, APlayerController*, PC, UInputAction*, InputAction, FInputActionValue, ActionValue);

bool FJumpLatentCommand::Update()
{
    return SimulateInput(PC, InputAction, ActionValue);
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel1");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    ATPSCharacter* Character = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if (!TestNotNull("Character exists", Character))
    {
        return false;
    }

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1))
    {
        return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Jump
    ADD_LATENT_AUTOMATION_COMMAND(
        FJumpLatentCommand(Cast<APlayerController>(Character->GetController()), Character->JumpAction, FInputActionValue(true)));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, World]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f));

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel2");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    ATPSCharacter* Character = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if (!TestNotNull("Character exists", Character))
    {
        return false;
    }

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1))
    {
        return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Jump
    ADD_LATENT_AUTOMATION_COMMAND(
        FJumpLatentCommand(Cast<APlayerController>(Character->GetController()), Character->JumpAction, FInputActionValue(true)));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, World]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f));

    return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel3");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    ATPSCharacter* Character = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
    if (!TestNotNull("Character exists", Character))
    {
        return false;
    }

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only three item exists", InventoryItems.Num(), 6))
    {
        return false;
    }

    //  Run forward for 3 seconds
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]()
        {
            SimulateInput(Cast<APlayerController>(Character->GetController()),  //
                Character->MoveAction,                                          //
                FInputActionValue(FInputActionValue::Axis2D(0.f, 1.f)));        //
        },                                                                      //
        []() {},                                                                //
        3.f));

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    // Jump
    ADD_LATENT_AUTOMATION_COMMAND(
        FJumpLatentCommand(Cast<APlayerController>(Character->GetController()), Character->JumpAction, FInputActionValue(true)));

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

    //  Run to right for 2 seconds
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]()
        {
            SimulateInput(Cast<APlayerController>(Character->GetController()),  //
                Character->MoveAction,                                          //
                FInputActionValue(FInputActionValue::Axis2D(1.f, 0.f)));        //
        },                                                                      //
        []() {},                                                                //
        2.f));

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [this, World]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

#endif

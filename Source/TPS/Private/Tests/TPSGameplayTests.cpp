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
#include "Tests/Utils/JsonUtils.h"
#include "Tests/Utils/InputRecordingUtils.h"
#include "EnhancedInputSubsystems.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsAreTakenOnRecordingMovement, "TPSGame.Gameplay.AllItemsAreTakenOnRecordingMovement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded, "TPSGame.Gameplay.MapsShouldBeLoaded",
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

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UEnhancedPlayerInput* InEnhancedPlayerInput, const TArray<FInputFrame>& InFrames)
        : World(InWorld), EnhancedPlayerInput(InEnhancedPlayerInput), Frames(InFrames)
    {
    }

    virtual bool Update() override
    {
        if (!World.IsValid() || !EnhancedPlayerInput.IsValid() || Frames.IsEmpty())
        {
            return true;
        }

        if (!bStarted)
        {
            bStarted = true;
            WorldStartTime = World->TimeSeconds;
        }

        const float ElapsedTime = World->TimeSeconds - WorldStartTime;

        while (Index + 1 < Frames.Num() && ElapsedTime >= Frames[Index + 1].TimeSeconds)
        {
            ++Index;
        }

        InjectFrame(Frames[Index]);

        return Index + 1 >= Frames.Num() && ElapsedTime >= Frames.Last().TimeSeconds;
    }

    void InjectFrame(const FInputFrame& Frame)
    {
        for (const FActionData& ActionData : Frame.ActionValues)
        {
            if (!ActionData.Action)
            {
                continue;
            }

            EnhancedPlayerInput->InjectInputForAction(ActionData.Action, ActionData.ToInputActionValue());
        }
    }

private:
    TWeakObjectPtr<UWorld> World;
    TWeakObjectPtr<UEnhancedPlayerInput> EnhancedPlayerInput;

    const TArray<FInputFrame> Frames;
    int32 Index{0};
    float WorldStartTime{0.f};
    bool bStarted{false};
};

void FAllItemsAreTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData  //
        {
            {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap", "CharacterTestInputMainMap.json"},  //
            {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap", "CharacterTestInputCustomMap.json"}    //
        };

    for (const auto& OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

bool FAllItemsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map name and JSON params should exist", ParsedParams.Num() == 2))
    {
        return false;
    }

    const auto Level = LevelScope(ParsedParams[0]);

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

    const FString FileName = GetTestDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData))
    {
        return false;
    }

    if (!TestTrue("InputData is not empty!", InputData.Frames.Num() > 0))
    {
        return false;
    }

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!TestNotNull("Player Controller exists", PlayerController))
    {
        return false;
    }

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

    // Simulate input movement
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, GetEnhancedPlayerInput(PlayerController), InputData.Frames));

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

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

void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    // const TTuple<FString, int32, bool, float> MyTuple = {"unreal", 5, true, 100.f};
    // const auto MyTuple2 = MakeTuple(FString("unity"), 7, false, 50.f);

    // auto str = MyTuple.Get<0>();
    // auto intgr = MyTuple.Get<1>();
    // auto bln = MyTuple.Get<2>();
    // auto flt = MyTuple.Get<3>();

    // UE_LOG(LogTemp, Display, TEXT("Tuple: %s, %i, %i, %f"), *str, intgr, bln, flt);

    // Tie(str, intgr, bln, flt) = MyTuple2;

    // UE_LOG(LogTemp, Display, TEXT("Tuple2: %s, %i, %i, %f"), *str, intgr, bln, flt);

    const TArray<TTuple<FString, FString>> TestData =  //
        {
            {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap"},  //
            {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap"}      //
        };

    for (const auto& OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Get<0>());
        OutTestCommands.Add(OneTestData.Get<1>());
    }
}

bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));

    return true;
}

#endif

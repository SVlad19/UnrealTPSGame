// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Weapon/TPSTurret.h"

using namespace TPS::Test;

BEGIN_DEFINE_SPEC(FTurret, "TPSGame.Turret",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

UWorld* World;
const FTransform InitialTransform{FVector{0.f, 286.f, 110.f}};
ATPSTurret* Turret;
END_DEFINE_SPEC(FTurret)

namespace
{
constexpr const char* MapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";
constexpr const char* TurretBPName = "/Game/Weapon/BP_TPSTurret";
constexpr const char* TurretBPTestName = "/Game/Tests/BP_TestTPSTurret";
}  // namespace

void FTurret::Define()
{
    Describe("Creations",
        [this]()  //
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                });

            It("Cpp instance can`t be created",
                [this]()
                {
                    const FString ExpectedWarningMsg =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);
                    Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn`t exists", Turret);
                });

            It("Blueprint instance can be created",
                [this]()
                {
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()  //
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.f}, {15, 3.f}, {5, 5.f}};
            for (const auto& Data : TestData)
            {
                const auto TestName =
                    FString::Printf(TEXT("Ammo: %i and freq: %.0f should be set up correctly"), Data.Get<0>(), Data.Get<1>());
                It(TestName,
                    [this, Data]()
                    {
                        const auto [Ammo, Freq] = Data;
                        CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});
                        const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                        TestTrueExpr(AmmoCount == Ammo);
                        const int32 FireFrequency = GetPropertyValueByName<ATPSTurret, float>(Turret, "FireFrequency");
                        TestTrueExpr(FireFrequency == Freq);
                    });
            }

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()  //
        {
            const int32 InitialAmmoCount = 3;
            const float FireFreq = 1.f;
            LatentBeforeEach(
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);

                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);

                    CallFuncByNameWithParams(
                        Turret, "SetTurretData", {FString::FromInt(InitialAmmoCount), FString::SanitizeFloat(FireFreq)});

                    TestDone.Execute();
                });

            LatentIt(FString::Printf(TEXT("Should be empty after %i sec"), FMath::RoundToInt(InitialAmmoCount * FireFreq)),
                EAsyncExecution::ThreadPool,
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == InitialAmmoCount);
                        });

                    const float SynchDelta = 0.5f;
                    FPlatformProcess::Sleep(InitialAmmoCount * FireFreq + SynchDelta);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == 0);
                        });

                    TestDone.Execute();
                });

            LatentAfterEach(
                [this](const FDoneDelegate& TestDone)
                {
                    SpecCloseLevel(World);
                    TestDone.Execute();
                });
        });
}

#endif

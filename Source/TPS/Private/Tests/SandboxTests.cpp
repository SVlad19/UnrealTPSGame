// My game copyright

#if WITH_AUTOMATION_TESTS
#include "Tests/SandboxTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sin",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");

    using MaxIntTestPayload = TArray<TPS::Test::TestPayload<TInterval<int32>, int32>>;

    const MaxIntTestPayload TestData{
        {{13, 25}, 25},
        {{25, 25}, 25},
        {{9, 123}, 123},
        {{0, 0}, 0},
        {{-2345, 0}, 0},
        {{-45, -67}, -45},
        {{-9, -9}, -9},
        {{-78, 34}, 34},
    };

    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
    }

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    // sqrt(3) = 1.73205

    // clang-format off
    TArray<TPS::Test::TestPayload<float, float>> SqrtTestPayload{
        {4.0f, 2.0f},
        {3.0f, 1.7f, 0.1f},
        {3.0f, 1.73f, 0.01f},
        {3.0f, 1.73205f, 1.e-5f}
    };
    //clang-format on

    for (const auto Data : SqrtTestPayload) {
        const bool IsEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue),Data.ExpectedValue, Data.Tolerance);
        TestTrueExpr(IsEqual);
    }

    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    // clang-format off
    using Degrees = float;
    TArray<TPS::Test::TestPayload<Degrees, float>> SinTestPayload{
        {Degrees(0.00f),0.0f},
        {Degrees(30.0f),0.5f},
        {Degrees(45.0f),0.707f},
        {Degrees(60.0f),0.866f},
        {Degrees(90.0f),1.0f}
    };
    //clang-format on

    for (const auto Data : SinTestPayload) {
        const float Rad = FMath::DegreesToRadians(Data.TestValue);
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Rad),Data.ExpectedValue,0.001f));
    }

    return true;
}

#endif
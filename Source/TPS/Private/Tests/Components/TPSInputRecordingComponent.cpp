// My game copyright

#include "Tests/Components/TPSInputRecordingComponent.h"
#include "Engine/World.h"
#include "Tests/TestUtils.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "Tests/Utils/JsonUtils.h"

using namespace TPS::Test;

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    check(GetOwner());
    check(GetWorld());

    if (const APawn* Pawn = Cast<APawn>(GetOwner()))
    {
        if (const APlayerController* PC = Pawn->GetController<APlayerController>())
        {
            EnhancedPlayerInput = TPS::Test::GetEnhancedPlayerInput(PC);
        }
    }

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Frames.Add(MakeInputFrame());
}

FActionData UTPSInputRecordingComponent::MakeActionData(UInputAction* Action, const FInputActionValue& InputValue) const
{
    FActionData Result;
    Result.Action = Action;
    Result.ValueType = InputValue.GetValueType();

    switch (Result.ValueType)
    {
        case EInputActionValueType::Boolean:
        {
            Result.Value.X = InputValue.Get<bool>() ? 1.f : 0.f;
            break;
        }

        case EInputActionValueType::Axis1D:
        {
            Result.Value.X = InputValue.Get<float>();
            break;
        }

        case EInputActionValueType::Axis2D:
        {
            const FVector2D AxisValue = InputValue.Get<FVector2D>();
            Result.Value.X = AxisValue.X;
            Result.Value.Y = AxisValue.Y;
            break;
        }

        case EInputActionValueType::Axis3D:
        {
            Result.Value = InputValue.Get<FVector>();
            break;
        }

        default: break;
    }

    return Result;
}

FInputFrame UTPSInputRecordingComponent::MakeInputFrame() const
{
    FInputFrame Frame;
    Frame.TimeSeconds = GetWorld()->TimeSeconds;
    if (!EnhancedPlayerInput)
    {
        return Frame;
    }

    for (UInputAction* Action : RecordedActions)
    {
        const FInputActionValue ActionValue = EnhancedPlayerInput->GetActionValue(Action);
        Frame.ActionValues.Add(MakeActionData(Action, ActionValue));
    }

    return Frame;
}

FString UTPSInputRecordingComponent::GenerateFileName() const
{
    FString SavedDir = FPaths::ProjectSavedDir();
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append("/Tests/").Append(FileName).Append("_").Append(Date).Append(".json");
}

void UTPSInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JsonUtils::WriteInputData(GenerateFileName(), InputData);
}

void UTPSInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Frames.Add(MakeInputFrame());
}

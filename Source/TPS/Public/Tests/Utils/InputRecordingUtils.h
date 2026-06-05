// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputRecordingUtils.generated.h"

USTRUCT()
struct FActionData
{
    GENERATED_BODY();

    UPROPERTY()
    TObjectPtr<UInputAction> Action{nullptr};

    UPROPERTY()
    EInputActionValueType ValueType{EInputActionValueType::Boolean};

    UPROPERTY()
    FVector Value{FVector::ZeroVector};

    FInputActionValue ToInputActionValue() const
    {
        switch (ValueType)
        {
            case EInputActionValueType::Boolean: return FInputActionValue(Value.X != 0.f);

            case EInputActionValueType::Axis1D: return FInputActionValue(static_cast<float>(Value.X));

            case EInputActionValueType::Axis2D: return FInputActionValue(FVector2D(Value.X, Value.Y));

            case EInputActionValueType::Axis3D: return FInputActionValue(Value);

            default: return FInputActionValue(false);
        }
    }
};

USTRUCT()
struct FInputFrame
{
    GENERATED_BODY();

    UPROPERTY()
    TArray<FActionData> ActionValues;

    UPROPERTY()
    float TimeSeconds{0.f};
};

USTRUCT()
struct FInputData
{
    GENERATED_BODY();

    UPROPERTY()
    TArray<FInputFrame> Frames;

    UPROPERTY()
    FTransform InitialTransform;
};

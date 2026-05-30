#if WITH_AUTOMATION_TESTS
#include "Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

namespace TPS
{
namespace Test
{

UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }

    return nullptr;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object)
    {
        return;
    }

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

bool SimulateInput(APlayerController* PC, UInputAction* InputAction, const FInputActionValue& ActionValue)
{
    if (!PC || !InputAction)
    {
        return false;
    }

    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer)
    {
        return false;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem)
    {
        return false;
    }

    UEnhancedPlayerInput* PlayerInput = Subsystem->GetPlayerInput();
    if (!PlayerInput)
    {
        return false;
    }

    PlayerInput->InjectInputForAction(InputAction, ActionValue);
    return true;
}

FTPSUntilLatentCommand::FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool FTPSUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }

    Callback();
    return false;
}

}  // namespace Test
}  // namespace TPS

#endif
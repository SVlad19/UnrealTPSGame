#if WITH_AUTOMATION_TESTS
#include "Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"
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

UEnhancedPlayerInput* GetEnhancedPlayerInput(const APlayerController* PC)
{
    if (!PC)
    {
        return nullptr;
    }

    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer)
    {
        return nullptr;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

    return Subsystem ? Subsystem->GetPlayerInput() : nullptr;
}

bool SimulateInput(APlayerController* PC, UInputAction* InputAction, const FInputActionValue& ActionValue)
{
    UEnhancedPlayerInput* PlayerInput = GetEnhancedPlayerInput(PC);
    if (!PlayerInput || !InputAction)
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

FString GetTestDataDir()
{
    return FPaths::GameSourceDir().Append("TPS/Data/");
}

UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
{
    if (!Widget || !Widget->WidgetTree)
    {
        return nullptr;
    }

    UWidget* FoundWidget = nullptr;
    UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
        [&](UWidget* Child)
        {
            if (Child && Child->GetFName().IsEqual(WidgetName))
            {
                FoundWidget = Child;
            }
        });

    return FoundWidget;
}

}  // namespace Test
}  // namespace TPS

#endif
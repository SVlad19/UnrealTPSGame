// My game copyright

#include "Character/TPSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATPSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(UIMappingContext, 0);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATPSPlayerController::ToggleGamePause);
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("'%s' Failed to find "
                 "an Enhanced Input "
                 "component! This "
                 "template is built to "
                 "use the "
                 "Enhanced Input "
                 "system. If you "
                 "intend to use the "
                 "legacy system, then "
                 "you will need to "
                 "update this C++ "
                 "file."),
            *GetNameSafe(this));
    }
}

void ATPSPlayerController::ToggleGamePause()
{
    bInPause = !bInPause;
    SetPause(bInPause);
    SetShowMouseCursor(bInPause);
    bInPause ? SetInputMode(FInputModeGameAndUI()) : SetInputMode(FInputModeGameOnly());

    OnGamePause.Broadcast(bInPause);
}

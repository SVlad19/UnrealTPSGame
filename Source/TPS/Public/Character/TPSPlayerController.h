// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

UCLASS()
class TPS_API ATPSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void ToggleGamePause();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> PauseAction;

protected:
    virtual void SetupInputComponent() override;

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> UIMappingContext;

    bool bInPause{false};
};

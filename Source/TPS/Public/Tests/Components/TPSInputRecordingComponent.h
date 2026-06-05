// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tests/Utils/InputRecordingUtils.h"
#include "TPSInputRecordingComponent.generated.h"

class UEnhancedPlayerInput;
class UInputAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInputRecordingComponent();

    inline const FInputData& GetInputData() const { return InputData; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere, Category = "Input Recording")
    TArray<TObjectPtr<UInputAction>> RecordedActions;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

private:
    FInputFrame MakeInputFrame() const;
    FActionData MakeActionData(UInputAction* Action, const FInputActionValue& InputValue) const;
    FString GenerateFileName() const;

    UPROPERTY()
    FInputData InputData;

    UPROPERTY()
    TObjectPtr<UEnhancedPlayerInput> EnhancedPlayerInput;
};

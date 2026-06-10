// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VideoSettingsWidget.generated.h"

class UVerticalBox;
class USettingOptionWidget;
class UButton;

UCLASS()
class TPS_API UVideoSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> VideoSettingsContainer;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> RunBenchmarkButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<USettingOptionWidget> SettingOptionWidgetClass;

private:
    UFUNCTION()
    void OnBenchmark();

    void OnVideoSettingsUpdated();
};

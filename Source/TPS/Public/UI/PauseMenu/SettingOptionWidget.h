// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingOptionWidget.generated.h"

class UTextBlock;
class UTPSGameSetting;
class UButton;

UCLASS()
class TPS_API USettingOptionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SettingDisplayName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SettingCurrentValue;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> NextSettingButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PrevSettingButton;

private:
    void Init(UTPSGameSetting* InSetting);
    void UpdateTexts();

    UFUNCTION()
    void OnNextSetting();

    UFUNCTION()
    void OnPrevSetting();

    TWeakObjectPtr<UTPSGameSetting> Setting;

    friend class UVideoSettingsWidget;
};

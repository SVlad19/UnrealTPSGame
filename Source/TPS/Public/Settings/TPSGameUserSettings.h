// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TPSGameUserSettings.generated.h"

class UTPSGameSetting;

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

UCLASS()
class TPS_API UTPSGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UTPSGameUserSettings();
    static UTPSGameUserSettings* Get();

    void RunBenchmark();

    FORCEINLINE const TArray<UTPSGameSetting*>& GetVideoSettings() const { return VideoSettings; }
    FOnSettingsUpdatedDelegate OnVideoSettingsUpdated;

private:
    UPROPERTY() TArray<UTPSGameSetting*> VideoSettings;
};

// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "TPSGameSetting.generated.h"

struct FSettingOption
{
    FText Name;
    int32 Value;
};

UCLASS()
class UTPSGameSetting : public UObject
{
    GENERATED_BODY()

public:
    FORCEINLINE void SetName(const FText& InName) { Name = InName; }
    FORCEINLINE void SetOptions(const TArray<FSettingOption>& InOptions) { Options = InOptions; }

    FORCEINLINE FText GetName() const { return Name; }
    FSettingOption GetCurrentOption() const;

    void AddGetter(TFunction<int32()> Func);
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyNextOption();
    void ApplyPrevOption();

private:
    int32 GetCurrentValue() const;
    void SetCurrentValue(int32 Value);
    int32 GetCurrentIndex() const;

    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;
};
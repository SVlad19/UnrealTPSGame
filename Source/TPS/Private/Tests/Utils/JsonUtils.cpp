// My game copyright

#include "Tests/Utils/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All);

using namespace TPS::Test;

bool JsonUtils::WriteInputData(const FString& FileName, const FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
    if (!MainJsonObject.IsValid())
    {
        return false;
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

    if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), JsonWriter))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON serialization failed"));
        return false;
    }

    if (!JsonWriter->Close())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON writer closing failed"));
        return false;
    }

    if (!FFileHelper::SaveStringToFile(OutputString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("File saving failed: %s"), *FileName);
        return false;
    }

    return true;
}

bool JsonUtils::ReadInputData(const FString& FileName, FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = MakeShareable(new FJsonObject());

    FString FileContnentString;
    if (!FFileHelper::LoadFileToString(FileContnentString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON serialization failed"));
        return false;
    }

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContnentString);
    if (!FJsonSerializer::Deserialize(JsonReader, MainJsonObject))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON deserialization failed"));
        return false;
    }

    return FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), &InputData, 0, 0);
}

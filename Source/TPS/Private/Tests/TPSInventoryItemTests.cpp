// My game copyright

#if WITH_AUTOMATION_TESTS
#include "Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/TPSInventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Tests/TestUtils.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TPS/TPSCharacter.h"
#include "Components/TPSInventoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.CollisionShouldBeSetupCorrectly",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
const char* InventoryItemBPName = "Blueprint'/Game/Inventory/BP_TPSInventoryItem.BP_TPSInventoryItem'";
const char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";

}  // namespace

using namespace TPS::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMsg =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);

    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.f}};
    const ATPSInventoryItem* InvItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitialTransform);
    if (!TestNull("Inventory item exists", InvItem))
    {
        return false;
    }

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.f}};
    const ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere component exists", CollisionComp))
    {
        return false;
    }

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render exists", TextRenderComp))
    {
        return false;
    }

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh exists", StaticMeshComp))
    {
        return false;
    }

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    // FVector Vector{120.f, 130.f, 140.f};
    // CallFuncByNameWithParams(InvItem, "SetTestData",
    //     {
    //         FString::FromInt(123),                                                   //
    //         FString::SanitizeFloat(345.45f),                                         //
    //         FString("\"My cool string\""),                                           //
    //         FString("Yes"),                                                          //
    //         FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Vector.X, Vector.Y, Vector.Z)  //
    //     });

    const FInventoryData InvData{EInventoryItemType::CYLINDER, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData",
        {
            InvData.ToString(),  //
            Color.ToString()     //
        });

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render exists", TextRenderComp))
    {
        return false;
    }

    TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InvData.Score)));
    TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh exists", StaticMeshComp))
    {
        return false;
    }

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exists", Material))
    {
        return false;
    }
    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);

    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    const int32 Score = 25;
    const EInventoryItemType Type = EInventoryItemType::CYLINDER;
    const FInventoryData InvData{Type, Score};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData",
        {
            InvData.ToString(),  //
            Color.ToString()     //
        });

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1))
    {
        return false;
    }

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull("Character exists", Character))
    {
        return false;
    }

    const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }
    TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == 0);

    // Character takes inventory item
    Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == Score);
    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InvItems);
    TestTrueExpr(InvItems.Num() == 0);

    return true;
}

#endif
// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTurret.generated.h"

class UStaticMeshComponent;
class ATPSProjectile;

UCLASS(Abstract)
class TPS_API ATPSTurret : public AActor
{
    GENERATED_BODY()

public:
    ATPSTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> TurretMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoCount{10};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = s))
    float FireFrequency{1.f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ATPSProjectile> ProjectionClass;

private:
    void OnFire();

    FTimerHandle FireTimerHandle;
};

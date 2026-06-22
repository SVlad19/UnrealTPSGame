// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class TPS_API ATPSProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATPSProjectile();

    inline void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageAmount{30.f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Units = s))
    float LifeSeconds{5.f};

private:
    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    FVector ShotDirection;
};

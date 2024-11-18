#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "CellType.h"

#include "GridSpecial.generated.h"

UCLASS()
class GRIDMODULE_API AGridSpecial : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AGridSpecial();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Box collision component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    ECellType CellType = ECellType::Impassable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    int32 Cost = 1;
};

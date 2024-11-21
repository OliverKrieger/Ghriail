#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PathfindingSimulator.generated.h"

UCLASS()
class DEBUGMODULE_API APathfindingSimulator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APathfindingSimulator();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Grid Indices for Start and End points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pathfinding")
    int32 StartIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pathfinding")
    int32 EndIndex;

    // Node positions that define the path
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pathfinding")
    TArray<FVector> NodePositions;

    // Spline Component for visualizing the path
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pathfinding")
    USplineComponent* SplineComponent;

    // Function to generate the spline
    UFUNCTION(BlueprintCallable, Category="Pathfinding")
    void GenerateSplineFromNodes();
};

#include "PathfindingSimulator.h"

// Sets default values
APathfindingSimulator::APathfindingSimulator()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the Spline Component
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;
}

// Called when the game starts or when spawned
void APathfindingSimulator::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APathfindingSimulator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Generate the spline from the NodePositions array
void APathfindingSimulator::GenerateSplineFromNodes()
{
    if (!SplineComponent || NodePositions.Num() == 0) return;

    SplineComponent->ClearSplinePoints(false);

    for (int32 i = 0; i < NodePositions.Num(); ++i)
    {
        SplineComponent->AddSplinePoint(NodePositions[i], ESplineCoordinateSpace::World, false);
    }

    // Update the spline after adding all points
    SplineComponent->UpdateSpline();
}

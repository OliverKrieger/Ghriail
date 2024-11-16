// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
    UE_LOG(GridModule_LogCategory, Log, TEXT("Grid construction called!"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent->SetMobility(EComponentMobility::Static);
    RootComponent->bVisualizeComponent = true;

    GridDebugVisualiser = CreateDefaultSubobject<UGridDebugVisualiserComponent>(TEXT("GridDebugVisualiser"));
}

void AGrid::UpdateGrid()
{
    //FlushPersistentDebugLines(GetWorld()); // flush before everything else so that update grid cells does not have to handle it and thus not clear other debug
    GridCells.Empty(); // Empty all cells before calculation
    GridCells.SetNum(GetGridSize1D());
    PerformRayTraceForTopCells();
    GridDebugVisualiser->UpdateDebugGridCells();
}


// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* ----------------------------- */
/* ----------------------------- */
/* LINE TRACING */
/* ----------------------------- */
/* ----------------------------- */

void AGrid::PerformRayTraceForTopCells()
{
    // Error check for if grid cells is being accessed before it has been intialised!
    if (GridCells.Num() == 0)
    {
        UE_LOG(GridModule_LogCategory, Error, TEXT("Grid Cell is empty, cannot perform cell calculation!"));
        return;
    }

    UE_LOG(GridModule_LogCategory, Log, TEXT("Begin grid cell calculation - Number of Cells to Calculate: %d"), GetGridSize1D());
    // Capture the top layer indices range
    int32 TopLayerStartIndex = GetTopLayerStartIndex();

    // ParallelFor to parallelize the outer loop
    ParallelFor(GetGridSize1D() - TopLayerStartIndex, [this, TopLayerStartIndex](int32 i)
        {
            // Calculate the top cell index
            int topHeightIndex = TopLayerStartIndex + i;
            UE_LOG(GridModule_LogCategory, Log, TEXT("Starting index draw for: %d"), topHeightIndex);

            // Perform downward ray trace for each height in this column
            for (int heightIterIndex = 0; heightIterIndex < Height; heightIterIndex++)
            {
                int32 currentIndex = topHeightIndex - heightIterIndex * (Depth * Width); // Calculate the 1D index

                // Perform the ray trace and update the grid cell
                PerformDownwardLineTrace(currentIndex);
            }
        });

    UE_LOG(GridModule_LogCategory, Log, TEXT("UPDATE COMPLETE - Grid cell calculation complete!"));
}

void AGrid::PerformDownwardLineTrace(const int32& GridIndex)
{
    FGridCell GridCellData;
    GridCellData.Cost = 1; // TODO cost dependant on Type
    GridCellData.Position = Convert3DGridPositionToWorld(Convert1DIndexTo3D(GridIndex)) + FVector(CellSize / 2); // move the position of the grid to the center of the box
    GridCellData.Type = PerformRaycast(GridCellData.Position + FVector(0, 0, CellSize / 2), GridIndex); // move the z to the top of the box

    if (GridIndex < GridCells.Num()) {
        GridCells[GridIndex] = GridCellData;
    }
    else {
        UE_LOG(GridModule_LogCategory, Error, TEXT("Line trace attempting to access grid index %d which is outside the bounds of the grid!"), GridIndex);
    }
}

ECellType AGrid::PerformRaycast(const FVector& worldStartPosition, const int32& GridIndex)
{
    FVector Start = worldStartPosition + FVector(0,0,-5); // adjust line trace distance from start so that it does not clip into geometry above
    FVector End = FVector(worldStartPosition.X, worldStartPosition.Y, (worldStartPosition.Z - CellSize));

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore the actor performing the raycast

    UWorld* World = this->GetWorld();
    FHitResult OutHit;

    bool bHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
    UE_LOG(GridModule_LogCategory, Log, TEXT("Drawn index for: %d | For world position: %s"), GridIndex, *worldStartPosition.ToString());

#if WITH_EDITOR
    if (bHit)
    {
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Green, false, LineTraceLifetime, 0, 1.0f);
        }

        // handle impassable
        int32 AboveGridIndex = GridIndex + (Width * Depth);
        if (AboveGridIndex < GetGridSize1D()) {
            if (GridCells[AboveGridIndex].Type == ECellType::Walkable || GridCells[AboveGridIndex].Type == ECellType::Impassable) {
                return ECellType::Impassable;
            }
        }

        return ECellType::Walkable; // TODO if previous walkable, impassable || TODO create class for blocker that has cell type and if that, put that down instead (rough terrain, etc)
    }
    else
    {
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Red, false, LineTraceLifetime, 0, 1.0f);
        }
        return ECellType::Air;
    }
#endif
}

/* ----------------------------- */
/* ----------------------------- */
/* PUBLIC HELPER FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */

FGridCell& AGrid::GetCell(int x, int y, int z)
{
    // Calculate the 1D index from the 3D coordinates
    int index = x + (y * Width) + (z * Width * Height);

    // Ensure the index is valid
    if (index >= 0 && index < GridCells.Num())
    {
        return GridCells[index];  // Return the reference to the FGridCell at the computed index
    }

    // Handle invalid access
    UE_LOG(GridModule_LogCategory, Error, TEXT("Invalid GridCell coordinates: (%d, %d, %d)"), x, y, z);

    // returns an uninitialised cell
    static FGridCell Uninitialised;
    Uninitialised.Type = ECellType::Uninitialised;
    return Uninitialised;
}

FVector AGrid::GetCellPosition(int32 X, int32 Y, int32 Z) const
{
    return FVector();
}

/* ----------------------------- */
/* ----------------------------- */
/* PRIVATE HELPER FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */

int32 AGrid::GetTopLayerStartIndex()
{
    return (Height - 1) * Width * Depth; // for instance if 4x4x4, then starting index in flat array is 48 (because array size is 64)
}

int32 AGrid::GetGridSize1D()
{
    return Depth * Width * Height;
}

FVector3f AGrid::GetGridSize3D()
{
    return FVector3f(Depth, Width, Height);
}

/* ----------------------------- */
/* ----------------------------- */
/* INTERFACE CALLS */
/* ----------------------------- */
/* ----------------------------- */
int32 AGrid::GetGridDepth() const
{
    return Depth;
}

int32 AGrid::GetGridWidth() const
{
    return Width;
}

int32 AGrid::GetGridHeight() const
{
    return Height;
}

int32 AGrid::GetGridSize() const
{
    return Depth * Width * Height;
}

float AGrid::GetGridCellSize() const
{
    return CellSize;
}

TArray<FGridCell> AGrid::GetGridCells() const
{
    return GridCells;
}

FVector AGrid::Convert1DIndexTo3D(const int32& index) const
{
    int32 z = index / (Depth * Width); // Height
    int32 y = (index % (Depth * Width)) / Depth; // Width
    int32 x = index % Depth; // Depth
    return FVector(x, y, z);
}

FVector AGrid::Convert3DGridPositionToWorld(const FVector& GridPosition) const
{
    return FVector(GridPosition.X * CellSize, GridPosition.Y * CellSize, GridPosition.Z * CellSize);
}

/* ----------------------------- */
/* ----------------------------- */
/* EDITOR SPECIFIC FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */

#if WITH_EDITOR
void AGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    UE_LOG(GridModule_LogCategory, Log, TEXT("Grid - Property change: %s"), *PropertyName.ToString());

    // Check if you remake the grid
    if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, Width) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, Height) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, Depth) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, CellSize))
    {
        UpdateGrid();
    }
}
void AGrid::OnConstruction(const FTransform& Transform)
{
    // Whenever the actor is placed, moved or modified
    Super::OnConstruction(Transform);
    UE_LOG(GridModule_LogCategory, Log, TEXT("Grid - On Construct called!"));
    if (OnContructionRunNumber < 2) {
        UpdateGrid();
        OnContructionRunNumber++;
    }
    
}
void AGrid::PostLoad()
{
    Super::PostLoad();
    UE_LOG(GridModule_LogCategory, Log, TEXT("Grid - Post Load called!"));
}
#endif
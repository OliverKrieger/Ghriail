// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InitializeGrid();
}

void AGrid::InitializeGrid()
{
    GridSize1D = Depth * Width * Height;
    GridCells.SetNum(GridSize1D);
    PerformRayTraceForTopCells();
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

FVector AGrid::Convert1DIndexTo3D(int32 index)
{
    int32 z = index / (Depth * Width); // Height
    int32 y = (index % (Depth * Width)) / Depth; // Width
    int32 x = index % Depth; // Depth
    return FVector(x, y, z);
}

FVector AGrid::ConvertGridPositionToWorld(FVector GridPosition)
{
    return FVector(GridPosition.X * CellSize, GridPosition.Y * CellSize, GridPosition.Z * CellSize);
}

/* ----------------------------- */
/* ----------------------------- */
/* LINE TRACING */
/* ----------------------------- */
/* ----------------------------- */

void AGrid::PerformRayTraceForTopCells()
{
    for (int topHeightIndex = GetTopLayerStartIndex(); topHeightIndex < GetGridSize1D(); topHeightIndex++)
    {
        for (int heightIterIndex = 0; heightIterIndex < Height; heightIterIndex++)
        {
            int32 currentIndex = topHeightIndex - heightIterIndex * (Depth * Width); // starting height - how far down the height are we
            PerformDownwardLineTrace(currentIndex);
        }
    }
}

void AGrid::PerformDownwardLineTrace(const int32& GridIndex)
{
    FGridCell GridCellData;
    GridCellData.Cost = 1; // TODO cost dependant on Type
    GridCellData.Position = ConvertGridPositionToWorld(Convert1DIndexTo3D(GridIndex));
    GridCellData.Type = PerformRaycast(GridCellData.Position);

    GridCells[GridIndex] = GridCellData;
}

ECellType AGrid::PerformRaycast(const FVector& worldStartPosition)
{
    FVector Start = worldStartPosition;
    FVector End = FVector(worldStartPosition.X, worldStartPosition.Y, (worldStartPosition.Z - CellSize));

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore the actor performing the raycast

    UWorld* World = this->GetWorld();
    FHitResult OutHit;

    bool bHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

#if WITH_EDITOR
    if (bHit)
    {
        DrawDebugLine(World, Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
        return ECellType::Walkable; // TODO if previous walkable, impassable || TODO create class for blocker that has cell type and if that, put that down instead (rough terrain, etc)
    }
    else
    {
        DrawDebugLine(World, Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
        return ECellType::Air;
    }
#endif
}

/* ----------------------------- */
/* ----------------------------- */
/* PRIVATE HELPER FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */

int32 AGrid::GetTopLayerStartIndex()
{
    return (Height-1) * Width * Depth; // for instance if 4x4x4, then starting index in flat array is 48 (because array size is 64)
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
/* PRIVATE DEBUG FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */
void AGrid::UpdateDebugGridCells()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Clear existing debug lines if bDrawDebugBoxes is set to false
    if (!bDrawDebugBoxes)
    {
        FlushPersistentDebugLines(GetWorld());
        return;
    }

    for (const FGridCell& Cell : GridCells)
    {
        FColor BoxColor;
        switch (Cell.Type)
        {
        case ECellType::Walkable:
            BoxColor = FColor::Yellow;
            break;
        case ECellType::Impassable:
            BoxColor = FColor::Red;
            break;
        case ECellType::Air:
            BoxColor = FColor::Blue;
            break;
        default:
            BoxColor = FColor::White;
            break;
        }

        DrawDebugBox(
            GetWorld(),
            Cell.Position,
            FVector(CellSize/2), // Half-size of the box
            BoxColor,
            true,
            -1.0f, // Persistent (set to DeltaTime for temporary drawing)
            0, // Depth Property
            2.0f // Thickness of lines
        );
    }
}

#if WITH_EDITOR
void AGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    // Check if the property that changed is bDrawDebugBoxes
    if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, bDrawDebugBoxes))
    {
        UpdateDebugGridCells();
    }
}
#endif
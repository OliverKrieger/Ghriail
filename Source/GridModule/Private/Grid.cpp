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
    GridCells.Empty(); // Empty all cells before calculation
    GridCells.SetNum(GetGridSize1D());
    PerformRayTraceForTopCells();
    AddAllNeighbours(); // After cells calculated, add neighbours, does take longer than adding indecies at line trace, but could not calculate types otherwise
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

            // Perform downward ray trace for each height in this column
            for (int heightIterIndex = 0; heightIterIndex < Height; heightIterIndex++)
            {
                int32 currentIndex = topHeightIndex - heightIterIndex * (Depth * Width); // Calculate the 1D index

                // Perform the ray trace and update the grid cell
                SetupDataForLineTrace(currentIndex);
            }
        });

    UE_LOG(GridModule_LogCategory, Log, TEXT("UPDATE COMPLETE - Grid cell calculation complete!"));
}

void AGrid::SetupDataForLineTrace(const int32& GridIndex)
{
    FGridCell GridCellData;
    GridCellData.Cost = 1; // TODO cost dependant on Type
    GridCellData.Position = Convert3DGridPositionToWorld(Convert1DIndexTo3D(GridIndex)) + FVector(CellSize / 2); // move the position of the grid to the center of the box
    PerformSingleLineTrace(GridIndex, GridCellData); // move the z to the top of the box

    if (GridIndex < GridCells.Num()) {
        GridCells[GridIndex] = GridCellData;
    }
    else {
        UE_LOG(GridModule_LogCategory, Error, TEXT("Line trace attempting to access grid index %d which is outside the bounds of the grid!"), GridIndex);
    }
}

void AGrid::PerformSingleLineTrace(const int32& GridIndex, FGridCell& GridCellData)
{
    FVector worldStartPosition = GridCellData.Position + FVector(0, 0, CellSize / 2); // Make the linetrace start from top of the box
    FVector Start = worldStartPosition + FVector(0,0,-5); // adjust line trace distance from start so that it does not clip into geometry above
    FVector End = FVector(worldStartPosition.X, worldStartPosition.Y, (worldStartPosition.Z - CellSize)); // take the x and y of the box, but move z to the bottom

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore the actor performing the raycast

    UWorld* World = this->GetWorld();
    FHitResult OutHit;

    bool bHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

    if (bHit)
    {
#if WITH_EDITOR
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Green, false, LineTraceLifetime, 0, 1.0f);
        }
#endif
        HandlePositionalInformation(GridCellData, OutHit);
        if (HandleSpecialHit(GridCellData, OutHit)) {
            return; // return early, as we already know type and cost
        }

        // Handle impassable by checking if above is
        // either walkable or impassable.
        // Not doing just is not air check, because special tiles
        // (might be stacked on top of each other)
        int32 AboveGridIndex = GridIndex + (Width * Depth);
        if (AboveGridIndex < GetGridSize1D()) { // check that this is not the top cell
            if (GridCells[AboveGridIndex].Type == ECellType::Walkable || GridCells[AboveGridIndex].Type == ECellType::Impassable) {
                GridCellData.Type = ECellType::Impassable;
                return;
            }
        }

        // If nothing above and we hit
        GridCellData.Type = ECellType::Walkable; // TODO create class for blocker that has cell type and if that, put that down instead (rough terrain, etc)
        return;
    }
    else
    {
#if WITH_EDITOR
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Red, false, LineTraceLifetime, 0, 1.0f);
        }
#endif
        GridCellData.Type = ECellType::Air;
        return;
    }
}

/* ----------------------------- */
/* ----------------------------- */
/* NEIGHBOUR CONNECTION FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */

void AGrid::AddAllNeighbours()
{
    for (int32 GridIndex = 0; GridIndex < GetGridSize1D(); GridIndex++) {
        AddNeighbours(GridIndex);
    }
}

void AGrid::AddNeighbours(const int32& GridIndex)
{
    FVector Grid3DIndex = Convert1DIndexTo3D(GridIndex);

    // Loop through all possible combinations of (-1, 0, 1) for x, y, z
    // All potential combinations, total of 27 with excluding (0,0,0):
    // (-1,-1,-1), (-1,-1,0), (-1,-1,1), (-1,0,-1), (-1,0,0), (-1,0,1), (-1,1,-1), (-1,1,0), (-1,1,1), (0,-1,-1), (0,-1,0), (0,-1,1), 
    // (0,0,-1), (0,0,1), (0,1,-1), (0,1,0), (0,1,1), (1,-1,-1), (1,-1,0), (1,-1,1), (1,0,-1), (1,0,0), (1,0,1), (1,1,-1), (1,1,0), (1,1,1)
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dz = -1; dz <= 1; ++dz)
            {
                // Skip the center cell (0, 0, 0)
                if (dx == 0 && dy == 0 && dz == 0)
                {
                    continue;
                }

                // Calculate new coordinates
                int NewX = Grid3DIndex.X + dx;
                int NewY = Grid3DIndex.Y + dy;
                int NewZ = Grid3DIndex.Z + dz;

                // Check if the new coordinates are within bounds
                if (NewX >= 0 && NewX < Width && NewY >= 0 && NewY < Depth && NewZ >= 0 && NewZ < Height)
                {
                    // Convert 3D coordinates to a 1D index
                    int NeighborIndex = NewX + NewY * Width + NewZ * Width * Depth;

                    // Each index is on it's own layer, so we know which type we are connected to.
                    // When traversing, we can easily filter the lists, by for instance, only taking 
                    // the walkable tiles or if we have a unit that can fly, we combine walkable and air
                    // indecies and traverse that.
                    if (GridCells[NeighborIndex].Type == ECellType::Walkable) {
                        GridCells[GridIndex].WalkableNeighbourIndecies.Add(NeighborIndex);
                    }
                    else if (GridCells[NeighborIndex].Type == ECellType::Air) {
                        GridCells[GridIndex].AirNeighbourIndecies.Add(NeighborIndex);
                    }
                    else if (GridCells[NeighborIndex].Type == ECellType::Impassable) {
                        GridCells[GridIndex].ImpassableNeighbourIndecies.Add(NeighborIndex);
                    }
                    else {
                        UEnum* EnumPtr = StaticEnum<ECellType>();
                        if (EnumPtr) {
                            FString EnumName = EnumPtr->GetNameByValue(static_cast<int64>(GridCells[NeighborIndex].Type)).ToString();
                            UE_LOG(GridModule_LogCategory, Warning, TEXT("WARNING - Encountered grid cell type while adding neighbours that is unrecognised: %s"), *EnumName);
                        }
                        else {
                            UE_LOG(GridModule_LogCategory, Warning, TEXT("WARNING - Encountered grid cell type while adding neighbours that is unrecognised, and enum metadata could not be found."));
                        }
                    }
                }
            }
        }
    }
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
    return FVector(); // TODO
}

void AGrid::RecalculateGrid()
{
    UpdateGrid();
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

FRotator AGrid::CalcRotationFromImpactNormal(const FVector& HitNormal)
{
    // Calculate a forward vector for the plane
    FVector ForwardVector = FVector::CrossProduct(FVector::UpVector, HitNormal).GetSafeNormal();

    // If the forward vector ends up being zero, choose a fallback direction
    if (ForwardVector.IsNearlyZero())
    {
        ForwardVector = FVector::CrossProduct(FVector::RightVector, HitNormal).GetSafeNormal();
    }

    // Create a rotation using the MakeFromXZ function and convert it to a rotator
    return FRotationMatrix::MakeFromXZ(ForwardVector, HitNormal).Rotator();
}

void AGrid::HandlePositionalInformation(FGridCell& GridCellData, const FHitResult& OutHit)
{
    GridCellData.Position = OutHit.Location; // Store hit location in world space
    FVector HitNormal = OutHit.ImpactNormal;

    // Calculate the dot product between the normal and the Z-axis
    // We are calculating the angle between the impact normal
    // and the line trace.
    float DotProduct = FVector::DotProduct(HitNormal, FVector(0, 0, 1));

    // Check if the hit surface is sloped (not flat)
    // We do not want to store a rotation if technically
    // There should not be one.
    const float SlopeThreshold = 0.99f; // Adjust this threshold if needed
    if (DotProduct < SlopeThreshold)
    {
        GridCellData.Rotation = CalcRotationFromImpactNormal(HitNormal); // Store rotation if not flat
    }
}

bool AGrid::HandleSpecialHit(FGridCell& GridCellData, const FHitResult& OutHit)
{
    AGridSpecial* HitSpecialGrid = Cast<AGridSpecial>(OutHit.GetActor());
    if (HitSpecialGrid)
    {
        UE_LOG(GridModule_LogCategory, Log, TEXT("Grid - Special hit!"));
        GridCellData.Type = HitSpecialGrid->CellType;
        GridCellData.Cost = HitSpecialGrid->Cost;
        return true;
    }

    return false;
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
    return (GridPosition * FVector(CellSize)) + GetActorLocation(); // first turn to 3D gird, then add offset
}

FVector AGrid::ConvertWorldTo3DGrid(const FVector& WorldPosition) const
{
    FVector Grid3DPosition = (WorldPosition + GetActorLocation()) / FVector(CellSize); // first remove offset, then divide by cell size to get grid index
    // Make sure the values are integers
    return FVector(
        FMath::RoundToInt(Grid3DPosition.X),
        FMath::RoundToInt(Grid3DPosition.Y),
        FMath::RoundToInt(Grid3DPosition.Z)
    );
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
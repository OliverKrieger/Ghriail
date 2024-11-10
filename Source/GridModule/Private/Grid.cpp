// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent->SetMobility(EComponentMobility::Static);
    RootComponent->bVisualizeComponent = true;

    HIMC_Air = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Air"));
    HIMC_Air->SetCullDistances(1000.0f, 10000.0f); // Adjust culling range
    HIMC_Air->bAffectDistanceFieldLighting = false; // Optimize lighting
    HIMC_Air->SetupAttachment(RootComponent);

    HIMC_Walkable = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Walkable"));
    HIMC_Walkable->SetCullDistances(1000.0f, 10000.0f);
    HIMC_Walkable->bAffectDistanceFieldLighting = false;
    HIMC_Walkable->SetupAttachment(RootComponent);

    HIMC_Impassable = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Impassable"));
    HIMC_Impassable->SetCullDistances(1000.0f, 10000.0f);
    HIMC_Impassable->bAffectDistanceFieldLighting = false;
    HIMC_Impassable->SetupAttachment(RootComponent);

    UpdateGrid();
}

void AGrid::UpdateGrid()
{
    //FlushPersistentDebugLines(GetWorld()); // flush before everything else so that update grid cells does not have to handle it and thus not clear other debug
    GridCells.Empty(); // Empty all cells before calculation
    GridSize1D = Depth * Width * Height;
    GridCells.SetNum(GridSize1D);
    PerformRayTraceForTopCells();
    if (bDrawDebugBoxes) {
        UpdateDebugGridCells();
    }
}


// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrid::BeginDestroy()
{
    Super::BeginDestroy();
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
    // Error check for if grid cells is being accessed before it has been intialised!
    if (GridCells.Num() == 0)
    {
        UE_LOG(GridModule_LogCategory, Error, TEXT("Grid Cell is empty, cannot perform cell calculation!"));
        return;
    }

    for (int topHeightIndex = GetTopLayerStartIndex(); topHeightIndex < GetGridSize1D(); topHeightIndex++)
    {
        for (int heightIterIndex = 0; heightIterIndex < Height; heightIterIndex++)
        {
            int32 currentIndex = topHeightIndex - heightIterIndex * (Depth * Width); // starting height - how far down the height are we
            PerformDownwardLineTrace(currentIndex);
        }
    }

    UE_LOG(GridModule_LogCategory, Log, TEXT("Grid Cell Calculation Complete"));
}

void AGrid::PerformDownwardLineTrace(const int32& GridIndex)
{
    FGridCell GridCellData;
    GridCellData.Cost = 1; // TODO cost dependant on Type
    GridCellData.Position = ConvertGridPositionToWorld(Convert1DIndexTo3D(GridIndex)) + FVector(CellSize/2); // move the position of the grid to the center of the box
    GridCellData.Type = PerformRaycast(GridCellData.Position+FVector(0,0,CellSize/2)); // move the z to the top of the box

    if (GridIndex < GridCells.Num()) {
        GridCells[GridIndex] = GridCellData;
    }
    else {
        UE_LOG(GridModule_LogCategory, Error, TEXT("Line trace attempting to access grid index %d which is outside the bounds of the grid!"), GridIndex);
    }
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
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
        }
        return ECellType::Walkable; // TODO if previous walkable, impassable || TODO create class for blocker that has cell type and if that, put that down instead (rough terrain, etc)
    }
    else
    {
        if (bDrawDebugRaytrace) {
            DrawDebugLine(World, Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
        }
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

void AGrid::AddHIMCVisualMesh(const FTransform& InstanceTransform, const ECellType& CellType)
{
    switch (CellType)
    {
    case ECellType::Air:
        if (HIMC_Air && HIMC_Air->GetStaticMesh())
        {
            HIMC_Air->AddInstance(InstanceTransform);
        }
        break;

    case ECellType::Walkable:
        if (HIMC_Walkable && HIMC_Walkable->GetStaticMesh())
        {
            FTransform Transform(InstanceTransform.GetLocation() - FVector(0, 0, CellSize / 2));
            HIMC_Walkable->AddInstance(Transform);
        }
        break;

    case ECellType::Impassable:
        if (HIMC_Impassable && HIMC_Impassable->GetStaticMesh())
        {
            HIMC_Impassable->AddInstance(InstanceTransform);
        }
        break;

    default:
        UE_LOG(GridModule_LogCategory, Warning, TEXT("Unknown cell type"));
        break;
    }
}

bool AGrid::HasMeshAvailability()
{
    if (!HIMC_Walkable || !HIMC_Walkable->GetStaticMesh()) {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("No mesh for grid walkable component!"));
        return false;
    }
    if (!HIMC_Impassable || !HIMC_Impassable->GetStaticMesh()) {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("No mesh for grid impassable component!"));
        return false;
    }
    if (!HIMC_Air || !HIMC_Air->GetStaticMesh()) {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("No mesh for grid air component!"));
        return false;
    }
    return true;
}

/* ----------------------------- */
/* ----------------------------- */
/* PRIVATE DEBUG FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */
void AGrid::UpdateDebugGridCells()
{
    if (!HasMeshAvailability()) {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("One or many grid mesh components not available!"));
        return;
    }

    HIMC_Air->ClearInstances();
    HIMC_Walkable->ClearInstances();
    HIMC_Impassable->ClearInstances();
    if (bDrawDebugBoxes) {
        for (const FGridCell& Cell : GridCells) {
            FVector Location(Cell.Position);
            FTransform Transform(Location);
            AddHIMCVisualMesh(Transform, Cell.Type);
        }
    }
}

#if WITH_EDITOR
void AGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    UE_LOG(GridModule_LogCategory, Log, TEXT("Property change: %s"), *PropertyName.ToString());

    // Check if the property that changed is bDrawDebugBoxes
    if (PropertyName == GET_MEMBER_NAME_CHECKED(AGrid, bDrawDebugBoxes))
    {
        UpdateDebugGridCells();
    }

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

    // If Debug enabled on construction
    if (bDrawDebugBoxes) {
        UpdateDebugGridCells();
    }

}
void AGrid::PostLoad()
{
    // Add anything here that you want post level load, warning however is that this can call twice at times
    Super::PostLoad();
}
#endif
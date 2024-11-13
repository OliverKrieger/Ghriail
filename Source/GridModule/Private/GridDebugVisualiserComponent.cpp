// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDebugVisualiserComponent.h"

// Sets default values for this component's properties
UGridDebugVisualiserComponent::UGridDebugVisualiserComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    // Get Parent
    GetParentActor();

    // Construct all of the HIMC components for visualisation
    HIMC_Air = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Air"));
    HIMC_Air->SetCullDistances(DebugVisualisationDistanceMin, DebugVisualisationDistanceMax); // Adjust culling range
    HIMC_Air->bAffectDistanceFieldLighting = false; // Optimize lighting
    HIMC_Air->SetMobility(EComponentMobility::Static);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinderAir(TEXT("/Game/Blockout/static/Cube_50.Cube_50"));
    if (MeshFinderAir.Succeeded())
    {
        HIMC_Air->SetStaticMesh(MeshFinderAir.Object);
        UE_LOG(GridModule_LogCategory, Log, TEXT("Mesh found for Air"));
    }
    else {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("Unable to find a mesh at path for Air"));
    }

    HIMC_Walkable = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Walkable"));
    HIMC_Walkable->SetCullDistances(DebugVisualisationDistanceMin, DebugVisualisationDistanceMax);
    HIMC_Walkable->bAffectDistanceFieldLighting = false;
    HIMC_Walkable->SetMobility(EComponentMobility::Static);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinderWalkable(TEXT("/Game/Blockout/static/Plane_90_Z1.Plane_90_Z1"));
    if (MeshFinderWalkable.Succeeded())
    {
        HIMC_Walkable->SetStaticMesh(MeshFinderWalkable.Object);
        UE_LOG(GridModule_LogCategory, Log, TEXT("Mesh found for Walkable"));
    }
    else {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("Unable to find a mesh at path for Walkable"));
    }

    HIMC_Impassable = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HIMC_Impassable"));
    HIMC_Impassable->SetCullDistances(DebugVisualisationDistanceMin, DebugVisualisationDistanceMax);
    HIMC_Impassable->bAffectDistanceFieldLighting = false;
    HIMC_Impassable->SetMobility(EComponentMobility::Static);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinderImpassable(TEXT("/Game/Blockout/static/Cube_100.Cube_100"));
    if (MeshFinderImpassable.Succeeded())
    {
        HIMC_Impassable->SetStaticMesh(MeshFinderImpassable.Object);
        UE_LOG(GridModule_LogCategory, Log, TEXT("Mesh found for Impassable"));
    }
    else {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("Unable to find a mesh at path for Impassable"));
    }
}


// Called when the game starts
void UGridDebugVisualiserComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGridDebugVisualiserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGridDebugVisualiserComponent::UpdateDebugGridCells()
{
    UE_LOG(GridModule_LogCategory, Log, TEXT("GridCells size: %d"), GridInterface->GetGridCells().Num());
    if (!HasMeshAvailability()) {
        UE_LOG(GridModule_LogCategory, Warning, TEXT("One or many grid mesh components not available!"));
        return;
    }

    UE_LOG(GridModule_LogCategory, Log, TEXT("Start updating debug grid cells"));
    UE_LOG(GridModule_LogCategory, Log, TEXT("Clearing all debug instances..."));

    // Clear all previous instances
    HIMC_Air->ClearInstances();
    HIMC_Walkable->ClearInstances();
    HIMC_Impassable->ClearInstances();

    UE_LOG(GridModule_LogCategory, Log, TEXT("Debug instance clear complete"));
    if (bDrawDebugBoxes) {
        
        UE_LOG(GridModule_LogCategory, Log, TEXT("Start drawing debug boxes"));

        // Hide all HIMC meshes while constructing
        HIMC_Air->SetVisibility(false);
        HIMC_Walkable->SetVisibility(false);
        HIMC_Impassable->SetVisibility(false);

        for (const FGridCell& Cell : GridInterface->GetGridCells()) {
            FVector Location(Cell.Position);
            FTransform Transform(Location);
            AddHIMCVisualMesh(Transform, Cell.Type);
        }

        // UnHide all HIMC meshes when complete
        HIMC_Air->SetVisibility(true);
        HIMC_Walkable->SetVisibility(true);
        HIMC_Impassable->SetVisibility(true);

        UE_LOG(GridModule_LogCategory, Log, TEXT("All debug instances drawn!"));
    }
}

bool UGridDebugVisualiserComponent::HasMeshAvailability()
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

void UGridDebugVisualiserComponent::AddHIMCVisualMesh(const FTransform& InstanceTransform, const ECellType& CellType)
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
            FTransform Transform(InstanceTransform.GetLocation() - FVector(0, 0, GridInterface->GetGridCellSize() / 2));
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

void UGridDebugVisualiserComponent::GetParentActor()
{
    AActor* ParentActor = GetOwner();

    if (ParentActor)
    {
        GridInterface = Cast<IGridInterface>(ParentActor);
        if (!GridInterface)
        {
            UE_LOG(GridModule_LogCategory, Error, TEXT("Parent actor is does not implement IGridInterface!"));
        }
    }
}

void UGridDebugVisualiserComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    UE_LOG(GridModule_LogCategory, Log, TEXT("Property change: %s"), *PropertyName.ToString());

    // Check if the property that changed is bDrawDebugBoxes
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UGridDebugVisualiserComponent, bDrawDebugBoxes))
    {
        UpdateDebugGridCells();
    }
}


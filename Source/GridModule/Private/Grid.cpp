// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

FGridCell& AGrid::GetCell(int x, int y, int z)
{
    // Calculate the 1D index from the 3D coordinates
    int index = x + (y * Width) + (z * Width * Height);

    // Ensure the index is valid
    if (index >= 0 && index < GridCells.Num())
    {
        return GridCells[index];  // Return the reference to the FGridCell at the computed index
    }

    // Handle invalid access (optional)
    UE_LOG(GridModule_LogCategory, Error, TEXT("Invalid GridCell coordinates: (%d, %d, %d)"), x, y, z);

    // returns an uninitialised cell
    static FGridCell Uninitialised;
    Uninitialised.Type = ECellType::Uninitialised;
    return Uninitialised;
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


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridModule_LogCategory.h"

#include "GridCell.h"

#include "Grid.generated.h"

UCLASS()
class GRIDMODULE_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	// Initializes the grid and performs ray tracing
	void InitializeGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* PUBLIC VALUES */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<FGridCell> GridCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Width = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Depth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Height = 1;

	int GridSize1D = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize = 100.f; // The size of each cell in the grid


	/* PUBLIC HELPER FUNCTIONS */

	// Access grid cells by (x, y, z) coordinates
	FGridCell& GetCell(int x, int y, int z);
	FVector GetCellPosition(int32 X, int32 Y, int32 Z) const;
	FVector Convert1DIndexTo3D(int32 index);
	FVector ConvertGridPositionToWorld(FVector GridPosition);

	/* DEBUG PUBLIC VALUES */

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugBoxes = false;

private:
	/* PRIVATE LINETRACE FUNCTIONS */
	void PerformRayTraceForTopCells();
	void PerformDownwardLineTrace(const int32& StartPosition);
	ECellType PerformRaycast(const FVector& worldStartPosition);

	/* PRIVATE HELPER FUNCTIONS */
	int32 GetTopLayerStartIndex();
	int32 GetGridSize1D();
	FVector3f GetGridSize3D();

	/* PRIVATE DEBUG FUNCTIONS */
	void UpdateDebugGridCells();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};

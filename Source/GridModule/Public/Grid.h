// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "GridModule_LogCategory.h"

#include "GridCell.h"
#include "IGridInterface.h"
#include "GridDebugVisualiserComponent.h"

#include "Grid.generated.h"

UCLASS()
class GRIDMODULE_API AGrid : public AActor, public IGridInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	// Initializes the grid and performs ray tracing
	void UpdateGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

/* ----------------------------- */
/* ----------------------------- */
/* VARIABLES */
/* ----------------------------- */
/* ----------------------------- */
	TArray<FGridCell> GridCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Width = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Depth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Height = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	float CellSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugRaytrace = false;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float LineTraceLifetime = 1.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Grid")
	UGridDebugVisualiserComponent* GridDebugVisualiser;

	// Non-BP
	
	// due to there being no good callback for a module that should be editor and runtime for when level is loaded, 
	// best fix is to wait for the contructor to run twice on editor load for first time instanciation. Alternative
	// would be to use OnLevelLoaded, but that would require adding editor module to the Grid Module, which could have
	// unforseen consquences for runtime.
	int OnContructionRunNumber = 0;

/* ----------------------------- */
/* ----------------------------- */
/* LINETRACE FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */
private:
	void PerformRayTraceForTopCells();
	void PerformDownwardLineTrace(const int32& StartPosition);
	ECellType PerformRaycast(const FVector& worldStartPosition, const int32& GridIndex);

/* ----------------------------- */
/* ----------------------------- */
/* HELPER FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */
public:
	// Access grid cells by (x, y, z) coordinates
	FGridCell& GetCell(int x, int y, int z);
	FVector GetCellPosition(int32 X, int32 Y, int32 Z) const;

private:
	int32 GetTopLayerStartIndex();
	int32 GetGridSize1D();
	FVector3f GetGridSize3D();

/* ----------------------------- */
/* ----------------------------- */
/* INTERFACE OVERRIDES */
/* ----------------------------- */
/* ----------------------------- */
public:
	virtual int32 GetGridDepth() const override;
	virtual int32 GetGridWidth() const override;
	virtual int32 GetGridHeight() const override;
	virtual int32 GetGridSize() const override;
	virtual float GetGridCellSize() const override;
	virtual TArray<FGridCell> GetGridCells() const override;
	virtual FVector Convert1DIndexTo3D(const int32& index) const override;
	virtual FVector Convert3DGridPositionToWorld(const FVector& GridPosition) const override;

/* ----------------------------- */
/* ----------------------------- */
/* EDITOR ONLY FUNCTIONS */
/* ----------------------------- */
/* ----------------------------- */
private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostLoad() override;
#endif

};

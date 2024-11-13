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

	/* PUBLIC VALUES */
	TArray<FGridCell> GridCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Width = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Depth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	int32 Height = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1"))
	float CellSize = 100.f;


	/* PUBLIC HELPER FUNCTIONS */

	// Access grid cells by (x, y, z) coordinates
	FGridCell& GetCell(int x, int y, int z);
	FVector GetCellPosition(int32 X, int32 Y, int32 Z) const;
	FVector Convert1DIndexTo3D(int32 index);
	FVector ConvertGridPositionToWorld(FVector GridPosition);

	/* DEBUG PUBLIC VALUES */

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugRaytrace = false;



	/* ----------------------------- */
	/* ----------------------------- */
	/* INTERFACE OVERRIDES */
	/* ----------------------------- */
	/* ----------------------------- */
	virtual int32 GetGridDepth() const override;
	virtual int32 GetGridWidth() const override;
	virtual int32 GetGridHeight() const override;
	virtual int32 GetGridSize() const override;
	virtual float GetGridCellSize() const override;
	virtual TArray<FGridCell> GetGridCells() const override;

private:
	/* ----------------------------- */
	/* ----------------------------- */
	/* PRIVATE DEBUG */
	/* ----------------------------- */
	/* ----------------------------- */
	UPROPERTY(VisibleAnywhere, Category = "Grid")
	UGridDebugVisualiserComponent* GridDebugVisualiser;

	/* ----------------------------- */
	/* ----------------------------- */
	/* PRIVATE LINETRACE FUNCTIONS */
	/* ----------------------------- */
	/* ----------------------------- */
	void PerformRayTraceForTopCells();
	void PerformDownwardLineTrace(const int32& StartPosition);
	ECellType PerformRaycast(const FVector& worldStartPosition);

	/* ----------------------------- */
	/* ----------------------------- */
	/* PRIVATE HELPER FUNCTIONS */
	/* ----------------------------- */
	/* ----------------------------- */
	int32 GetTopLayerStartIndex();
	int32 GetGridSize1D();
	FVector3f GetGridSize3D();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

};

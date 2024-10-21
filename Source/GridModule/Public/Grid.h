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

	// Access grid cells by (x, y, z) coordinates
	FGridCell& GetCell(int x, int y, int z);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<FGridCell> GridCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Width = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Length = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int Height = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize = 100.f; // The size of each cell in the grid
};

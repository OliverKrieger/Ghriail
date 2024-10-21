// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellType.h"

#include "GridCell.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
    GENERATED_BODY()

    // Example of UPROPERTY if you need it exposed to Unreal
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float Cost;

    // Enumeration for cell types (walkable, impassable, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    ECellType Type;

    FGridCell() {
        Position = FVector(.0f, .0f, .0f);
        Cost = 1;
        Type = ECellType::Impassable;
    }
};
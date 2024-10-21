// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellType.generated.h"

UENUM(BlueprintType)
enum class ECellType : uint8
{
    Walkable UMETA(DisplayName = "Walkable"),
    Air UMETA(DisplayName = "Air"),
    Impassable UMETA(DisplayName = "Impassable"),
    DifficultTerrain UMETA(DisplayName = "Difficult Terrain"),
    Uninitialised UMETA(DisplayName = "Uninitialised")
};
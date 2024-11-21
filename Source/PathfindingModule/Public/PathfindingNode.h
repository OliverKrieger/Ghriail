// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PathfindingNode.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
    int32 Index; // 1D index in your grid array

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
    float GCost; // Cost from the start node

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
    float HCost; // Heuristic cost to the target node

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
    int32 ParentIndex; // Index of the parent node in the path

    // Default constructor
    FPathfindingNode()
        : Index(-1), GCost(0.0f), HCost(0.0f), ParentIndex(-1) {}

    // Parameterized constructor
    FPathfindingNode(int32 index, float gcost, float hcost, int32 parentindex)
        : Index(index), GCost(gcost), HCost(hcost), ParentIndex(parentindex) {}
};
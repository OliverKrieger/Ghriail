#pragma once

#include "CoreMinimal.h"

#include "GridCell.h"

#include "GridMathUtils.h"

#include "PathfindingModule_LogCategory.h"

#include "PriorityQueue.h"

#include "PathfindingManager.generated.h"

UCLASS()
class PATHFINDINGMODULE_API UPathfindingManager : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    UPathfindingManager();

    float CalculateHeuristic(const FIntVector& Start, const FIntVector& Target);
    TArray<FIntVector> PerformAStar(const TArray<FGridCell>& Grid, int32 Width, int32 Height, int32 Depth, const FIntVector& StartGrid3DIndex, const FIntVector& EndGrid3DIndex);

};

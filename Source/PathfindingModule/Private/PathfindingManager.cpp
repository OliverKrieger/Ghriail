#include "PathfindingManager.h"

UPathfindingManager::UPathfindingManager()
{

}

float UPathfindingManager::CalculateHeuristic(const FIntVector& Start, const FIntVector& Target)
{
    return FVector::Dist2D(FVector(Start), FVector(Target));
}

TArray<FIntVector> UPathfindingManager::PerformAStar(
    const TArray<FGridCell>& GridCells, 
    int32 GridWidth, int32 GridHeight, int32 GridDepth, 
    const FIntVector& StartGrid3DIndex, const FIntVector& EndGrid3DIndex)
{
    int32 Start1DIndex = FGridMathUtils::Convert3DIndexTo1D(StartGrid3DIndex, GridWidth, GridDepth);
    int32 End1DIndex = FGridMathUtils::Convert3DIndexTo1D(EndGrid3DIndex, GridWidth, GridDepth);

    UPriorityQueue OpenList;
    TSet<int32> ClosedList;

    OpenList.Push({ Start1DIndex, 0.0f, CalculateHeuristic(StartGrid3DIndex, EndGrid3DIndex), -1 });

    while (!OpenList.IsEmpty())
    {
        FPathfindingNode Current = OpenList.Pop();

        if (Current.Index == End1DIndex) // if true, goal reached
        {
            // Reconstruct path
            TArray<FIntVector> Path;
            int32 PathIndex = Current.Index;
            while (PathIndex != -1)
            {
                Path.Add(FGridMathUtils::Convert1DIndexTo3D(PathIndex, GridWidth, GridDepth));
                PathIndex = Current.ParentIndex;
            }
            Algo::Reverse(Path);
            return Path;
        }

        // Add current node to the closed list
        ClosedList.Add(Current.Index);

        // Access precomputed walkable neighbors
        TArray<int32> Neighbors = GridCells[Current.Index].WalkableNeighbourIndecies;

        for (int32 NeighborIndex : Neighbors)
        {
            if (ClosedList.Contains(NeighborIndex)) continue;

            const FGridCell& NeighborCell = GridCells[NeighborIndex];

            // Skip impassable neighbors
            if (NeighborCell.Type == ECellType::Impassable) continue;

            // Calculate tentative G cost
            float TentativeGCost = Current.GCost + CalculateHeuristic(
                FGridMathUtils::Convert1DIndexTo3D(Current.Index, GridWidth, GridDepth),
                FGridMathUtils::Convert1DIndexTo3D(NeighborIndex, GridWidth, GridDepth)
            );

            // Construct the neighbor node
            FPathfindingNode NeighborNode = {
                NeighborIndex,
                TentativeGCost,
                CalculateHeuristic(FGridMathUtils::Convert1DIndexTo3D(NeighborIndex, GridWidth, GridDepth), EndGrid3DIndex),
                Current.Index
            };

            // Add the neighbor node to the open list
            OpenList.Push(NeighborNode);
        }
    }

    return TArray<FIntVector>(); // Return an empty array if no path found.
}
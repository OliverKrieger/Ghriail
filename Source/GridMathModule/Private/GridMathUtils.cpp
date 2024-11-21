#include "GridMathUtils.h"

FIntVector FGridMathUtils::Convert1DIndexTo3D(const int32 Grid1DIndex, int32 Width, int32 Depth)
{
    int32 Z = Grid1DIndex / (Depth * Width);
    int32 Y = (Grid1DIndex % (Depth * Width)) / Depth;
    int32 X = Grid1DIndex % Depth;
    return FIntVector(X, Y, Z);
}

int32 FGridMathUtils::Convert3DIndexTo1D(const FIntVector& Grid3DIndex, int32 Width, int32 Depth)
{
    return Grid3DIndex.X + (Grid3DIndex.Y * Depth) + (Grid3DIndex.Z * Depth * Width);
}

FVector FGridMathUtils::Convert3DGridIndexToWorldPos(const FIntVector& Grid3DIndex, const FVector& GridOffsetLocation, const float& CellSize)
{
    return FVector(Grid3DIndex * FIntVector(CellSize)) + GridOffsetLocation; // first turn to 3D grid index to world by multiplying with cell size, then add offset to world
}

FIntVector FGridMathUtils::ConvertWorldPosTo3DGridIndex(const FVector& WorldPosition, const FVector& GridOffsetLocation, const float& CellSize)
{
    FVector Grid3DPosition = (WorldPosition - GridOffsetLocation) / FVector(CellSize); // first remove world offset, then divide by cell size to get grid index
    // Make sure the values are integers
    return FIntVector(
        FMath::RoundToInt(Grid3DPosition.X),
        FMath::RoundToInt(Grid3DPosition.Y),
        FMath::RoundToInt(Grid3DPosition.Z)
    );
}

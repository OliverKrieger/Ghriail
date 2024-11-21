#pragma once

#include "CoreMinimal.h"

class GRIDMATHMODULE_API FGridMathUtils
{
public:
    // Converts a 1D index to a 3D index
    static FIntVector Convert1DIndexTo3D(const int32 Grid1DIndex, int32 Width, int32 Depth);

    // Converts a 3D index to a 1D index
    static int32 Convert3DIndexTo1D(const FIntVector& Grid3DIndex, int32 Width, int32 Depth);

    // Converts a 3D index to a world position
    static FVector Convert3DGridIndexToWorldPos(const FIntVector& Grid3DIndex, const FVector& GridOffsetLocation, const float& CellSize);

    // Converts a world position to a 3D index
    static FIntVector ConvertWorldPosTo3DGridIndex(const FVector& WorldPosition, const FVector& GridOffsetLocation, const float& CellSize);
};

// IGridInterface.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridCell.h"

#include "IGridInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGridInterface : public UInterface
{
    GENERATED_BODY()
};

class GRIDMODULE_API IGridInterface
{
    GENERATED_BODY()

public:
    virtual int32 GetGridDepth() const = 0;
    virtual int32 GetGridWidth() const = 0;
    virtual int32 GetGridHeight() const = 0;
    virtual int32 GetGridSize() const = 0;
    virtual float GetGridCellSize() const = 0;
    virtual TArray<FGridCell> GetGridCells() const = 0;
    virtual FIntVector Convert1DIndexTo3D(const int32& Grid1DIndex) const = 0;
    virtual int32 Convert3DIndexTo1D(const FIntVector& Grid3DIndex) const = 0;
    virtual FVector Convert3DGridIndexToWorldPos(const FIntVector& Grid3DIndex) const = 0;
    virtual FIntVector ConvertWorldPosTo3DGridIndex(const FVector& WorldPosition) const = 0;
};
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
    virtual FVector Convert1DIndexTo3D(const int32& index) const = 0;
    virtual FVector Convert3DGridPositionToWorld(const FVector& GridPosition) const = 0;
};
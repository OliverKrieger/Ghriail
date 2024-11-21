#pragma once

#include "CoreMinimal.h"

#include "PathfindingNode.h"

#include "PriorityQueue.generated.h"

UCLASS()
class PATHFINDINGMODULE_API UPriorityQueue : public UObject
{
    GENERATED_BODY()

public:

    UPriorityQueue();

    TArray<FPathfindingNode> Nodes;

    void Push(const FPathfindingNode& Node);
    FPathfindingNode Pop();
    bool IsEmpty();
};

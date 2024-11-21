#include "PriorityQueue.h"

UPriorityQueue::UPriorityQueue(){

}

void UPriorityQueue::Push(const FPathfindingNode& Node)
{
	Nodes.Add(Node);
	Nodes.Sort([](const FPathfindingNode& A, const FPathfindingNode& B) {
		return (A.GCost + A.HCost) < (B.GCost + B.HCost);
	});
}

FPathfindingNode UPriorityQueue::Pop()
{
	FPathfindingNode Node = Nodes[0];
	Nodes.RemoveAt(0);
	return Node;
}

bool UPriorityQueue::IsEmpty()
{
	return Nodes.Num() == 0;
}


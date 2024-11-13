// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/SceneComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "GridModule_LogCategory.h"

#include "GridCell.h"
#include "IGridInterface.h"

#include "GridDebugVisualiserComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDMODULE_API UGridDebugVisualiserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridDebugVisualiserComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ----------------------------- */
	/* ----------------------------- */
	/* DEBUG PUBLIC VALUES */
	/* ----------------------------- */
	/* ----------------------------- */
	
	// Transient so specifically DrawDebugBoxes is not loaded on construction
	// and manually has to be activated by the user. This is to avoid slow
	// load time on editor open for large grids, but also because in editor
	// the grid loads before the collisions are loaded.
	UPROPERTY(EditAnywhere, Category = "Debug Visualisation", Transient)
	bool bDrawDebugBoxes = false;

	UPROPERTY(EditAnywhere, Category = "Debug Visualisation")
	float DebugVisualisationDistanceMin = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Debug Visualisation")
	float DebugVisualisationDistanceMax = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Debug Visualisation")
	bool bShowAirTiles = true;

	UPROPERTY(EditAnywhere, Category = "Debug Visualisation")
	bool bShowWalkableTiles = true;

	UPROPERTY(EditAnywhere, Category = "Debug Visualisation")
	bool bShowImpassableTiles = true;

	/* ----------------------------- */
	/* ----------------------------- */
	/* DEBUG PUBLIC FUNCTIONS */
	/* ----------------------------- */
	/* ----------------------------- */
	void UpdateDebugGridCells();
	bool HasMeshAvailability();
	void AddHIMCVisualMesh(const FTransform& InstanceTransform, const ECellType& CellType);

	
private:
	/* ----------------------------- */
	/* ----------------------------- */
	/* DEBUG PRIVATE VALUES */
	/* ----------------------------- */
	/* ----------------------------- */

	UHierarchicalInstancedStaticMeshComponent* HIMC_Air;
	UHierarchicalInstancedStaticMeshComponent* HIMC_Walkable;
	UHierarchicalInstancedStaticMeshComponent* HIMC_Impassable;

	// Non-BP
	IGridInterface* GridInterface;
	TArray<UHierarchicalInstancedStaticMeshComponent*> HIMComponents;

	/* ----------------------------- */
	/* ----------------------------- */
	/* DEBUG PRIVATEFUNCTIONS */
	/* ----------------------------- */
	/* ----------------------------- */
	void GetParentActor();
	void SetHIMCComponentParameters(UHierarchicalInstancedStaticMeshComponent* HISMComponent);
	UHierarchicalInstancedStaticMeshComponent* CreateHIMComponent(FName ComponentName, FString MeshPath, FString MaterialPath);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};

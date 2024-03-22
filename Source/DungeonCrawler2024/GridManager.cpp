// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"

TMap<ETileTypes, UInstancedStaticMeshComponent*> ISMMap;

// Sets default values and other construction things
AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initializing Grid Sizes
	GridWidth = 5;
	GridHeight = 5;


	
}

// Called when the object is placed in the editor or spawned. 
void AGridManager::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	// Sizing the grid array to hold all tiles
	Grid.SetNum(GridWidth * GridHeight);


	
	// TODO: Initializing the Instanced Static Meshes for each TileMesh in the blueprint
	for (int i = 0; i < TileMeshes.Num(); ++i)
	{
		// TODO: Get Key for current entry
		ETileTypes key = ;
		UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>();
		ISM->RegisterComponent();
		// Insert static mesh from relevent TileMeshes map entry
		ISM->SetStaticMesh(TileMeshes[key]);
		ISMMap[key] = ISM;
	}

	// Generating the grid of tiles.
	for (int XTile = 0; XTile < GridWidth; ++XTile)
	{
		for (int YTile = 0; YTile < GridHeight; ++YTile)
		{
			// Get the tile reference then set it's type
			FGridTileStruct Tile = GetTile(XTile, YTile);
			Tile.TileType = ETileTypes::floor;
			//TODO: Add instances to the relevent mesh.
		}
	}
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	ISM = NewObject<UStaticMeshComponent>(this);
}


// Returns the tile struct from the grid for a given x and y coordinate
FGridTileStruct AGridManager::GetTile(int X, int Y) const
{
	checkf(X < GridWidth, TEXT("Get Tile had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Get Tile had been passed a Y value greater than grid height."));

	return Grid[X + (Y * GridWidth)];
}

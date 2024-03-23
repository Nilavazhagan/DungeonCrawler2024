// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"

TMap<ETileTypes, UInstancedStaticMeshComponent*> ISMMap;
TArray<ETileTypes> TileKeyArray;

// Sets default values and other construction things
AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initializing Grid Sizes
	GridWidth = 5;
	GridHeight = 5;
	GridTileSize = 100;

	
}

// Called when the object is placed in the editor or spawned. 
void AGridManager::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	// Clear all instances on all Instanced Static Mesh Components
	for (int i = 0; i < TileKeyArray.Num(); ++i)
	{
		ETileTypes Key = TileKeyArray[i];
		if (ISMMap.Contains(Key))
		{
			ISMMap[Key]->ClearInstances();
		}
	}

	// Add instances based on the tile type of each grid square
	Grid.SetNum(GridWidth * GridHeight);
	for (int YTile = 0; YTile < GridHeight; ++YTile)
	{
		for (int XTile = 0; XTile < GridWidth; ++XTile)
		{
			FGridTileStruct Tile = GetTile(XTile, YTile);
			if (TileKeyArray.Contains(Tile.TileType) == false)
			{
				Tile.TileType = ETileTypes::floor;
			}

			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			// Adding an instance mesh of the tiles type
			ISMMap[Tile.TileType]->AddInstance(FTransform(TilePosition));
		}
	}
}


void AGridManager::PostActorCreated()
{
	Super::PostActorCreated();

	// Initializing the Instanced Static Meshes for each TileMesh entry in the blueprint
	TileMeshes.GenerateKeyArray(TileKeyArray);
	ISMMap.Empty();
	for (int i = 0; i < TileKeyArray.Num(); ++i)
	{
		// Get Key for current entry
		ETileTypes Key = TileKeyArray[i];
		UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(this);
		ISM->SetupAttachment(GetRootComponent());
		ISM->RegisterComponent();
		// Insert static mesh from relevent TileMeshes map entry
		ISM->ClearInstances();
		ISM->SetStaticMesh(TileMeshes[Key]);
		ISMMap.Add(Key, ISM);
		this->AddInstanceComponent(ISM);
	}

	// Generating the grid of tiles.
	Grid.Empty();
	for (int YTile = 0; YTile < GridHeight; ++YTile)
	{
		for (int XTile = 0; XTile < GridWidth; ++XTile)
		{
			// Get the tile reference then initialize it's type
			FGridTileStruct Tile = FGridTileStruct{};
			Tile.TileType = ETileTypes::floor;
			Grid.Add(Tile);
			// Calculate the position of the new tile
			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			//Add instances to the relevent ISM component.
			ISMMap[Tile.TileType]->AddInstance(FTransform(TilePosition));
		}
	}
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}


// Returns the tile struct from the grid for a given x and y coordinate
FGridTileStruct AGridManager::GetTile(int X, int Y)
{
	checkf(X < GridWidth, TEXT("Get Tile had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Get Tile had been passed a Y value greater than grid height."));
	return Grid[X + (Y * GridWidth)];
}

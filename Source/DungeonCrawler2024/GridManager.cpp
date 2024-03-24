// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values and other construction things
AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initializing Grid Sizes
	GridWidth = 3;
	GridHeight = 3;
	GridTileSize = 100;

	// Filled manually so it can be read even before TileMeshes Key values can be accessed
	// (Important for loading from disk)
	TileKeyArray = {
		ETileTypes::floor,
		ETileTypes::wall
	};
}

// Called when the object is placed in the editor or spawned. 
void AGridManager::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	
	// Clear all instances on all Instanced Static Mesh Components for a clean slate
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
			FGridTileStruct Tile{};
			GetTile(Tile, XTile, YTile);

			if (!TileKeyArray.Contains(Tile.TileType))
			{
				Tile.TileType = TileKeyArray[0];
			}

			double TileXPosition = XTile * GridTileSize + this->GetActorLocation().X;
			double TileYPosition = YTile * GridTileSize + this->GetActorLocation().Y;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);

			if (ISMMap.Contains(Tile.TileType))
			{
				ISMMap[Tile.TileType]->AddInstance(FTransform(TilePosition));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("A tile's type has no correlated InstancedStaticMesh component!"));
			}
		}
	}
}


void AGridManager::PostActorCreated()
{
	Super::PostActorCreated();
	CreateGrid();
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}


// Returns a reference to the tile struct from the grid for a given x and y coordinate
FGridTileStruct& AGridManager::GetTile(FGridTileStruct& Tile, int X, int Y)
{
	checkf(X < GridWidth, TEXT("Get Tile had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Get Tile had been passed a Y value greater than grid height."));
	Tile = Grid[X + (Y * GridWidth)];
	return Tile;
}

void AGridManager::CreateGrid()
{
	// Initializing the Instanced Static Meshes for each TileMesh entry in the blueprint
	for (int i = 0; i < TileKeyArray.Num(); ++i)
	{
		// Get Key for current entry
		ETileTypes Key = TileKeyArray[i];
		UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(this);
		ISM->RegisterComponent();
		// Insert static mesh from relevent TileMeshes map entry
		ISM->SetStaticMesh(TileMeshes[Key]);
		// Add it to the manager and store a reference
		this->AddInstanceComponent(ISM);
		ISMMap.Add(Key, ISM);
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
			// Calculate the position of the tile
			double TileXPosition = XTile * GridTileSize + this->GetActorLocation().X;
			double TileYPosition = YTile * GridTileSize + this->GetActorLocation().Y;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			
			// If there is an ISM for this tile type, create the instance
			if (ISMMap.Contains(Tile.TileType))
			{
				ISMMap[Tile.TileType]->AddInstance(FTransform(TilePosition));
			}
		}
	}
}
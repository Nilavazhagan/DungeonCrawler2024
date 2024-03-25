// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ImageCore.h"

// Sets default values and other construction things
AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting up the root scene component
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;


	// Initializing Grid Sizes
	GridWidth = 100;
	GridHeight = 100;
	GridTileSize = 100;

	// Filled manually so it can be read even before Key values can be accessed
	// (Important for loading from disk)
	TileKeyArray = {
		ETileTypes::floor,
		ETileTypes::wall
	};

	// Setting which tile types are mapped to which color
	ColorToTileMap.Add(FColor::White, ETileTypes::floor);
	ColorToTileMap.Add(FColor::Black, ETileTypes::wall);

	// Initializing the Instanced Static Meshes for each TileMesh entry in the blueprint
	for (int i = 0; i < TileKeyArray.Num(); ++i)
	{
		ETileTypes Key = TileKeyArray[i];
		// Creating the InstancedStaticMesh component as a subobject under the manager's root component
		FString NewComponentName = FString::Printf(TEXT("InstancedStaticMeshComponent %i"), i);
		UInstancedStaticMeshComponent* ISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName(NewComponentName));
		ISM->RegisterComponent();
		ISM->SetupAttachment(RootComponent);
		ISM->InstancingRandomSeed = 69;
		this->AddInstanceComponent(ISM);
		// Adding the ISM to a map for easy access when needed
		ISMMap.Add(Key, ISM);
	}

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

	// If user is applying changes to the grid, update selected tiles' data
	if (bApply) {
		// This will make the checkbox work as a simple button
		bApply = false;
		// Clamp input ranges to grid size
		SelectionTile1[0] = FMath::Clamp(SelectionTile1[0], 0, GridWidth - 1);
		SelectionTile1[1] = FMath::Clamp(SelectionTile1[1], 0, GridHeight - 1);
		SelectionTile2[0] = FMath::Clamp(SelectionTile2[0], 0, GridWidth - 1);
		SelectionTile2[1] = FMath::Clamp(SelectionTile2[1], 0, GridHeight - 1);

		// Update a range of tiles
		if (bMultiSelect)
		{
			int StartX = FMath::Min(SelectionTile1[0], SelectionTile2[0]);
			int EndX = FMath::Max(SelectionTile1[0], SelectionTile2[0]);
			int StartY = FMath::Min(SelectionTile1[1], SelectionTile2[1]);
			int EndY = FMath::Max(SelectionTile1[1], SelectionTile2[1]);

			for (int X = StartX; X <= EndX; ++X)
			{
				for (int Y = StartY; Y <= EndY; ++Y)
				{
					SetTileType(AppliedTileType, X, Y);
				}
			}
		}
		// Update a single tile
		else
		{
			SetTileType(AppliedTileType, SelectionTile1[0], SelectionTile1[1]);
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

			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);

			Tile.Position = TilePosition;

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
	if (bIsGeneratedFromImage)
	{
		checkf(GenerationImage != NULL, TEXT("Generate from image was active with no generation image assigned!"));
		GenerateFromImage();
	}
	else
	{
		CreateGrid();
	}
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

// Returns a reference to the tile that is closest to the given world space location
FGridTileStruct& AGridManager::GetClosestTile(FGridTileStruct& Tile, FVector Location)
{
	// Use world position to calculate closest grid coordinate, and if the coordinate is off the grid,
	// clamp it to the valid coord.
	int XGridCoord = FMath::RoundHalfToZero(Location.X / GridTileSize);
	XGridCoord = FMath::Clamp(XGridCoord, 0, GridWidth);
	int YGridCoord = FMath::RoundHalfToZero(Location.Y / GridTileSize);
	YGridCoord = FMath::Clamp(YGridCoord, 0, GridHeight);

	GetTile(Tile, XGridCoord, YGridCoord);
	return Tile;
}

// Generate the grid structs and fill relevant data
void AGridManager::CreateGrid()
{
	// Generating the grid of tiles.
	Grid.Empty();
	for (int YTile = 0; YTile < GridHeight; ++YTile)
	{
		for (int XTile = 0; XTile < GridWidth; ++XTile)
		{
			// Get the tile reference then initialize it's type
			FGridTileStruct Tile = FGridTileStruct{};
			Tile.TileType = TileKeyArray[0];
			Tile.ActorsOccupying = TSet<AActor*>();
			Grid.Add(Tile);

			// Calculate the position of the tile
			double TileXPosition = XTile * GridTileSize + this->GetActorLocation().X;
			double TileYPosition = YTile * GridTileSize + this->GetActorLocation().Y;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			Tile.Position = TilePosition;
		}
	}
}

// Read the Manager's GenerationImage texture image to create tiles and assign types
void AGridManager::GenerateFromImage()
{
	FTexture2DMipMap MippityMappity = GenerationImage->GetPlatformData()->Mips[0];
	const FColor* Pixels = static_cast<const FColor*>(MippityMappity.BulkData.LockReadOnly());

	GridWidth = MippityMappity.SizeX;
	GridHeight = MippityMappity.SizeY;

	// Generating the grid of tiles.
	Grid.Empty();
	for (int YTile = 0; YTile < GridHeight; ++YTile)
	{
		for (int XTile = 0; XTile < GridWidth; ++XTile)
		{
			// Get the pixel data
			FColor CurrentPixel = Pixels[XTile + (YTile * GridWidth)];

			// Get the tile reference then initialize it's type
			FGridTileStruct Tile = FGridTileStruct{};
			Tile.TileType = ColorToTileMap[CurrentPixel];
			Tile.ActorsOccupying = TSet<AActor*>();
			Grid.Add(Tile);

			// Calculate the position of the tile
			double TileXPosition = XTile * GridTileSize + this->GetActorLocation().X;
			double TileYPosition = YTile * GridTileSize + this->GetActorLocation().Y;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			Tile.Position = TilePosition;
		}
	}


	MippityMappity.BulkData.Unlock();
}

bool AGridManager::MoveActor(AActor* Actor, FVector Direction)
{
	FGridTileStruct CurrentTile;
	for (int i = 0; i < Grid.Num(); i++ )
	{
		FGridTileStruct TileStruct = Grid[i];
		if (TileStruct.ActorsOccupying.Contains(Actor))
		{
			CurrentTile = TileStruct;
			break;
		}
	}

	FGridTileStruct NewTile;
	if (GetAdjacentTile(CurrentTile, Direction, NewTile))
	{
		NewTile.ActorsOccupying.Add(Actor);
		CurrentTile.ActorsOccupying.Remove(Actor);

		const FVector NewPosition = FVector(NewTile.Position.X, NewTile.Position.Y, Actor->GetActorLocation().Z);
		Actor->SetActorLocation(NewPosition);
	}


	return true;
}

bool AGridManager::GetAdjacentTile(FGridTileStruct& SourceTile, FVector Direction, FGridTileStruct& AdjacentTile)
{
	const FVector CurrentPosition = SourceTile.Position;
	const FVector NewPosition = CurrentPosition + (Direction.Normalize() * GridTileSize);

	for(int i = 0; i < Grid.Num(); i++)
		if (Grid[i].Position == NewPosition)
		{
			AdjacentTile = Grid[i];
			return true;
		}
	
	return false;
}

// Registers an actor to their closest tile
void AGridManager::RegisterActor(AActor* Actor)
{
	FVector DesiredTilePosition = Actor->GetActorLocation();
	DesiredTilePosition.Z = GetActorLocation().Z;
	
	FGridTileStruct Tile{};
	Tile = GetClosestTile(Tile, DesiredTilePosition);
	Tile.ActorsOccupying.Add(Actor);
}

// Set Tile Type
void AGridManager::SetTileType(ETileTypes Type, int X, int Y)
{
	checkf(X < GridWidth, TEXT("Set Tile Type had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Set Tile Type had been passed a Y value greater than grid height."));
	Grid[X + (Y * GridWidth)].TileType = Type;
}
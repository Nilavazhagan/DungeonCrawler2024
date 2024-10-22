// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

#include "TickActorInterface.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TileBlockingComponent.h"

// Sets default values and other construction things
AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setting up the root scene component
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	// Initializing Grid Sizes
	GridWidth = 100;
	GridHeight = 100;
	GridTileSize = 100;
	TileCenterOffset = FVector2D(0,0);

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
		ISM->SetupAttachment(RootComponent);
		ISM->InstancingRandomSeed = 69;
		this->AddInstanceComponent(ISM);
		// Adding the ISM to a map for easy access when needed
		ISMMap.Add(Key, ISM);
	}

}

// Called when the object is placed in the editor, spawned, or a property gets changed. 
void AGridManager::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	bIsFirstTick = true;
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
			FGridTileStruct& Tile = GetTile(XTile, YTile);

			if (!TileKeyArray.Contains(Tile.TileType))
			{
				Tile.TileType = TileKeyArray[0];
			}

			// Setting the Tile's position
			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			UE_LOG(LogTemp, Display, TEXT("Setting tile (%i, %i)'s position to (%f, %f, %f)"), XTile, YTile, TilePosition.X, TilePosition.Y, TilePosition.Z);
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

void AGridManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsFirstTick)
		OnGameActorTickComplete();
	bIsFirstTick = false;
}

// Called when the actor is spawned into the world
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
	numWaitingForActors = 0;
	bIsPlayerTurn = true;
	TickActors.Empty();
}


// Returns a copy of the tile struct from the grid for a given x and y coordinate
FGridTileStruct& AGridManager::GetTile(int X, int Y)
{
	checkf(X < GridWidth, TEXT("Get Tile had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Get Tile had been passed a Y value greater than grid height."));
	return Grid[X + (Y * GridWidth)];
}

// Returns a copy to the tile that is closest to the given world space location
FGridTileStruct& AGridManager::GetClosestTile(FVector Location)
{
	// Use world position to calculate closest grid coordinate, and if the coordinate is off the grid,
	// clamp it to the valid coord.
	int XGridCoord = FMath::RoundHalfToZero(Location.X / GridTileSize);
	XGridCoord = FMath::Clamp(XGridCoord, 0, GridWidth);
	int YGridCoord = FMath::RoundHalfToZero(Location.Y / GridTileSize);
	YGridCoord = FMath::Clamp(YGridCoord, 0, GridHeight);

	return GetTile(XGridCoord, YGridCoord);
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
			FGridTileStruct Tile{};
			Tile.TileType = TileKeyArray[0];
			Tile.ActorsOccupying = TSet<AActor*>();

			// Calculate the position of the tile
			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			Tile.Position = TilePosition;

			Grid.Add(Tile);
		}
	}
}

// Read the Manager's GenerationImage texture image to create tiles and assign types
void AGridManager::GenerateFromImage()
{
	// Getting the image data and locking it to avoid corruption during generation
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
			FGridTileStruct Tile{};
			Tile.TileType = ColorToTileMap[CurrentPixel];
			Tile.ActorsOccupying = TSet<AActor*>();

			// Calculate the position of the tile
			double TileXPosition = XTile * GridTileSize;
			double TileYPosition = YTile * GridTileSize;
			FVector TilePosition = FVector(TileXPosition, TileYPosition, 0.0);
			Tile.Position = TilePosition;

			Grid.Add(Tile);
		}
	}

	// Now we're done, so it is safe to unlock
	MippityMappity.BulkData.Unlock();
}

void AGridManager::RegenerateFromImage()
{
	GenerateFromImage();
	OnConstruction(this->GetActorTransform());
}

FGridTileStruct& AGridManager::GetAdjacentTileInDirection(FVector Location, FVector Direction)
{
	Direction.Normalize();
	FVector NewLocation = Location + (Direction * GridTileSize);
	FGridTileStruct& NewTile = GetClosestTile(NewLocation);
	return NewTile;
}

bool AGridManager::IsTileBlocking(FGridTileStruct Tile)
{
	// Block movement into walls
	if (Tile.TileType == ETileTypes::wall)
	{
		return true;
	}

	// Check if the NewTile has any actors that block movement
	for (AActor* OccupyingActor : Tile.ActorsOccupying)
	{
		UTileBlockingComponent* TileBlocker = OccupyingActor->GetComponentByClass<UTileBlockingComponent>();
		if (TileBlocker && TileBlocker->bDoesBlockTile)
		{
			return true;
		}
	}

	return false;
}

// The grid manager moves and re-registers actor from its current tile to an adjacent one based on direction.
// Checks for movement blocking
bool AGridManager::MoveActor(AActor* Actor, FVector Direction)
{
	const FVector OldLocation = Actor->GetActorLocation();
	FGridTileStruct& NewTile = GetAdjacentTileInDirection(OldLocation, Direction);

	return MoveActorTo(Actor, NewTile);
}

bool AGridManager::MoveActorTo(AActor* Actor, FGridTileStruct& ToTile)
{
	const FVector OldLocation = Actor->GetActorLocation();
	FGridTileStruct& OldTile = GetClosestTile(OldLocation);
	if (IsTileBlocking(ToTile))
		return false;

	// Adjust player position to be in center of tile
	Actor->SetActorLocation(
		FVector(ToTile.Position.X + TileCenterOffset.X,
				ToTile.Position.Y + TileCenterOffset.Y,
				OldLocation.Z));

	// Re-register actor at the new tile.
	OldTile.ActorsOccupying.Remove(Actor);
	ToTile.ActorsOccupying.Add(Actor);

	return true;
}

// Registers an actor to their closest tile
void AGridManager::RegisterActor(AActor* Actor)
{
	FVector DesiredTilePosition = Actor->GetActorLocation();

	FGridTileStruct& Tile = GetClosestTile(DesiredTilePosition);

	Tile.ActorsOccupying.Add(Actor);

	const FVector CurrentLocation = Actor->GetActorLocation();

	// Adjust player position to be in center of tile
	Actor->SetActorLocation(
		FVector(Tile.Position.X + TileCenterOffset.X,
				Tile.Position.Y + TileCenterOffset.Y,
				CurrentLocation.Z));

	UTileBlockingComponent* TileBlockingComponent = Cast<UTileBlockingComponent>(
		Actor->GetComponentByClass(UTileBlockingComponent::StaticClass()));
	TileBlockingComponent->OnDestroy.BindDynamic(this, &AGridManager::UnregisterActor);

	const ITickActorInterface* TickActor = Cast<ITickActorInterface>(Actor);
	if (TickActor == nullptr)
		return;

	TickActors.Add(Actor);
}

void AGridManager::UnregisterActor(AActor* Actor)
{
	for (FGridTileStruct& Tile : Grid)
	{
		if (!Tile.ActorsOccupying.IsEmpty() && Tile.ActorsOccupying.Contains(Actor))
		{
			Tile.ActorsOccupying.Remove(Actor);
			UTileBlockingComponent* TileBlockingComponent = Cast<UTileBlockingComponent>(
				Actor->GetComponentByClass(UTileBlockingComponent::StaticClass()));
			TileBlockingComponent->OnDestroy.Unbind();

			const ITickActorInterface* TickActor = Cast<ITickActorInterface>(Actor);
			if (TickActor == nullptr)
				return;

			TickActors.Remove(Actor);
			break;
		}
	}
}

// Set Tile Type
void AGridManager::SetTileType(ETileTypes Type, int X, int Y)
{
	checkf(X < GridWidth, TEXT("Set Tile Type had been passed an X value greater than grid width."));
	checkf(Y < GridHeight, TEXT("Set Tile Type had been passed a Y value greater than grid height."));
	Grid[X + (Y * GridWidth)].TileType = Type;
}

void AGridManager::OnGameActorTickComplete()
{
	if (numWaitingForActors > 0 && --numWaitingForActors != 0)
		return;

	for(int i = 0, count = TickActors.Num(); i < count; i++)
	{
		if (!TickActors.IsValidIndex(i))
			continue;
		
		AActor* Actor = TickActors[i];
		if (!IsValid(Actor))
			continue;

		ITickActorInterface* TickActor = Cast<ITickActorInterface>(Actor);
		if (TickActor == nullptr)
			continue;
		
		if (bIsPlayerTurn)
			TickActor->CallPlayerTickDelayed();
		else
			TickActor->CallEnemyTickDelayed();
		numWaitingForActors++;
	}
	bIsPlayerTurn = !bIsPlayerTurn;
}
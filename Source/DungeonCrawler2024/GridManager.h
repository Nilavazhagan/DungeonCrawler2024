// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"


// This represents each type of grid tile that can be spawned
UENUM()
enum class ETileTypes
{
	empty,
	floor,
	wall
};


// Each tile has a type and should hold a reference to all actors occupying it
USTRUCT()
struct FGridTileStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Position;

	UPROPERTY(EditInstanceOnly, SimpleDisplay)
	ETileTypes TileType;

	UPROPERTY()
	TSet<AActor*> ActorsOccupying;
};


UCLASS()
class DUNGEONCRAWLER2024_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridManager();

	// The number of tiles for the grid's x and y dimensions
	UPROPERTY(EditInstanceOnly)
	int GridWidth;
	UPROPERTY(EditInstanceOnly)
	int GridHeight;
	// The width and height of any given tile
	UPROPERTY(EditAnywhere)
	float GridTileSize = 100;
	UPROPERTY(EditAnywhere)
	FVector2D TileCenterOffset;



	// Using an image to generate grid
	UPROPERTY(EditAnywhere, Category = "GridEditing")
	bool bIsGeneratedFromImage = false;
	UPROPERTY(EditAnywhere, Category = "GridEditing")
	UTexture2D* GenerationImage;


	// Grid Editing Interface Stuff
	UPROPERTY(EditInstanceOnly, EditFixedSize, Category = "GridEditing|ManualEdit")
	TArray<int> SelectionTile1{ 0, 0 };
	UPROPERTY(EditInstanceOnly, Category = "GridEditing|ManualEdit", meta = (DisplayName = "MultiSelect"))
	bool bMultiSelect{ false };
	UPROPERTY(EditInstanceOnly, EditFixedSize, Category = "GridEditing|ManualEdit")
	TArray<int> SelectionTile2{ 0, 0 };
	UPROPERTY(EditInstanceOnly, Category = "GridEditing|ManualEdit", meta = (DisplayName = "Tile Type"))
	ETileTypes AppliedTileType{ ETileTypes::empty };
	// bApply is a bool checkbox that can work like a simple button
	UPROPERTY(EditInstanceOnly, Category = "GridEditing|ManualEdit", meta = (DisplayName = "Apply"))
	bool bApply{ false };


	// An array to hold tile data
	UPROPERTY()
	TArray<FGridTileStruct> Grid;

	// Returns reference of the tile struct at a given x and y coordinate
	UFUNCTION()
	FGridTileStruct& GetTile(int X, int Y);

	// Returns reference of the closest grid tile to a given world location
	UFUNCTION()
	FGridTileStruct& GetClosestTile(FVector Location);
	// Creates a fresh grid with all tiles set to the default type
	UFUNCTION()
	void CreateGrid();

	UFUNCTION()
	bool MoveActor(AActor* Actor, FVector Direction);

	UFUNCTION()
	bool GetAdjacentTile(FGridTileStruct& SourceTile, FVector Direction, FGridTileStruct& AdjacentTile);

	UFUNCTION()
	void RegisterActor(AActor* Actor);

	// Set a Tile's type in the grid
	UFUNCTION()
	void SetTileType(ETileTypes Type, int X, int Y);

	// Generates a grid, using pixels from a texture to determine tile types
	UFUNCTION()
	void GenerateFromImage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the actor is spawned into the world
	virtual void PostActorCreated();
	// Called when the actor is placed or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	TMap<ETileTypes, UInstancedStaticMeshComponent*> ISMMap;
	TArray<ETileTypes> TileKeyArray;
	TMap<FColor, ETileTypes> ColorToTileMap;

};

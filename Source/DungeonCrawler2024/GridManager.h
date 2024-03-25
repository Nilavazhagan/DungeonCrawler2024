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
	
	UPROPERTY()
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

	// An array to hold tile data
	UPROPERTY()
	TArray<FGridTileStruct> Grid;




	// Returns the tile struct at a given x and y coordinate
	UFUNCTION()
	FGridTileStruct& GetTile(FGridTileStruct& Tile, int X, int Y);

	UFUNCTION()
	FGridTileStruct& GetClosestTile(FGridTileStruct& Tile, FVector Location);

	UFUNCTION()
	void CreateGrid();

	UFUNCTION()
	bool MoveActor(AActor* Actor, FVector Direction);

	UFUNCTION()
	bool GetAdjacentTile(FGridTileStruct& SourceTile, FVector Direction, FGridTileStruct& AdjacentTile);

	UFUNCTION()
	void RegisterActor(AActor* Actor);


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
};

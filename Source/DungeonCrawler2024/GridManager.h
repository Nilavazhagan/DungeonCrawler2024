// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"


// This represents each type of grid tile that can be spawned
UENUM()
enum class ETileTypes
{
	floor,
	wall
};


// Each tile has a type and should hold a reference to all actors occupying it
USTRUCT()
struct FGridTileStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETileTypes TileType;

	UPROPERTY(VisibleAnywhere)
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
	UPROPERTY(EditAnywhere)
	TArray<FGridTileStruct> Grid;
	// A map for which static mesh to use for each tile type
	UPROPERTY(EditDefaultsOnly)
	TMap<ETileTypes, UStaticMesh*> TileMeshes;

	// Returns the tile struct at a given x and y coordinate
	UFUNCTION()
	FGridTileStruct GetTile(int X, int Y) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the actor is placed or spawned
	virtual void OnConstruction(const FTransform& Transform);
};
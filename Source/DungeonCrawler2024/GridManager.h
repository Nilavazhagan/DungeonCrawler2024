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

	// Grid Editing Interface Stuff
	UPROPERTY(EditInstanceOnly, EditFixedSize, Category = "GridEditing")
	TArray<int> SelectionTile1{0, 0};
	UPROPERTY(EditInstanceOnly, Category="GridEditing")
	bool bMultiSelect{ false };
	UPROPERTY(EditInstanceOnly, EditFixedSize, Category="GridEditing")
	TArray<int> SelectionTile2{ 0, 0 };
	UPROPERTY(EditInstanceOnly, Category = "GridEditing", meta = (DisplayName = "Tile Type"))
	ETileTypes AppliedTileType{ETileTypes::empty};
	// bApply is a bool checkbox that can work like a simple button
	UPROPERTY(EditInstanceOnly, Category="GridEditing")
	bool bApply{ false };

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

	UFUNCTION()
	void SetTileType(ETileTypes Type, int X, int Y);


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

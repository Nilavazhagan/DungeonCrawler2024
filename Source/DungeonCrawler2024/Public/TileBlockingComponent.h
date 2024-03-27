// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileBlockingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRAWLER2024_API UTileBlockingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileBlockingComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Blocking", meta = (DisplayName = "Does Block Tile"))
	bool bDoesBlockTile;

protected:


public:	

		
};

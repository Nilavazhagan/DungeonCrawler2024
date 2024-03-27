// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBlockingComponent.h"

#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTileBlockingComponent::UTileBlockingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bDoesBlockTile = true;
}

void UTileBlockingComponent::BeginPlay()
{
	Super::BeginPlay();

	const ADungeonCrawler2024GameMode* GameMode = Cast<ADungeonCrawler2024GameMode, AGameModeBase>(
		UGameplayStatics::GetGameMode(this->GetWorld()));
	GameMode->GridManager->RegisterObject(this);
}


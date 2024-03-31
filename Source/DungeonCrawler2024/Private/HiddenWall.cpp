// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenWall.h"


// Sets default values
AHiddenWall::AHiddenWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileBlockingComponent = CreateDefaultSubobject<UTileBlockingComponent>(TEXT("TileBlockingComponent"));
}

// Called when the game starts or when spawned
void AHiddenWall::BeginPlay()
{
	Super::BeginPlay();
	TileBlockingComponent->RegisterComponent();
	if(bIsHidden == true)
	{
		RootComponent->SetVisibility(false, true);
		TileBlockingComponent->bDoesBlockTile = false;
	}
}

// Called every frame
void AHiddenWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHiddenWall::Toggle()
{
	const bool bShouldHide = !bIsHidden;
	RootComponent->SetVisibility(!bShouldHide, true);
	TileBlockingComponent->bDoesBlockTile = !bShouldHide;
	bIsHidden = bShouldHide;
}


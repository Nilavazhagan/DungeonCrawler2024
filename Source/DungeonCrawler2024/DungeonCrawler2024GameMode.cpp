// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonCrawler2024GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADungeonCrawler2024GameMode::ADungeonCrawler2024GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ADungeonCrawler2024GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GridManager = Cast<AGridManager, AActor>(
		UGameplayStatics::GetActorOfClass(this, AGridManager::StaticClass()));
}

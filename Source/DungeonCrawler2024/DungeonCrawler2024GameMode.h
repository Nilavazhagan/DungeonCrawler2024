// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"
#include "GameFramework/GameModeBase.h"
#include "DungeonCrawler2024GameMode.generated.h"

UCLASS(minimalapi)
class ADungeonCrawler2024GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;

public:
	ADungeonCrawler2024GameMode();

	// Assigned GridManager should be read-only outside of this class
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	AGridManager* GridManager;
};




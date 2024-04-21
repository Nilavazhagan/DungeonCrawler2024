// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TickActorInterface.generated.h"

class AGridManager;
// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UTickActorInterface : public UInterface
{
	GENERATED_BODY()
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTickComplete);

/**
 * 
 */
class DUNGEONCRAWLER2024_API ITickActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual AGridManager* GetGridManager() = 0;
	UFUNCTION()
	virtual void CallPlayerTickDelayed() = 0;
	UFUNCTION()
	virtual void CallEnemyTickDelayed() = 0;
	
	UFUNCTION()
	virtual void OnPlayerTick();

	UFUNCTION()
	virtual void OnEnemyTick();

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastTickComplete();
};

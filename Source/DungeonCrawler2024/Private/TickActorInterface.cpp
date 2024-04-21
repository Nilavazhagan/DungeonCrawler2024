// Fill out your copyright notice in the Description page of Project Settings.


#include "TickActorInterface.h"

#include "DungeonCrawler2024/GridManager.h"

// Add default functionality here for any ITickActorInterface functions that are not pure virtual.
void ITickActorInterface::OnPlayerTick()
{
	BroadcastTickComplete();
}

void ITickActorInterface::OnEnemyTick()
{
	BroadcastTickComplete();
}

void ITickActorInterface::BroadcastTickComplete()
{
	GetGridManager()->OnGameActorTickComplete();
}

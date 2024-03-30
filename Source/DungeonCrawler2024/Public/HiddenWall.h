// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBlockingComponent.h"
#include "GameFramework/Actor.h"
#include "HiddenWall.generated.h"

UCLASS()
class DUNGEONCRAWLER2024_API AHiddenWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHiddenWall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHidden;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Toggle();

private:
	
	UPROPERTY()
	UTileBlockingComponent* TileBlockingComponent;
};

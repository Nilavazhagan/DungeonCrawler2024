// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HiddenWall.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerCharacter.h"
#include "HiddenWallTrigger.generated.h"

UENUM()
enum class TriggerMode
{
	Once,
	LookAway,
	Everytime
};

UCLASS()
class DUNGEONCRAWLER2024_API AHiddenWallTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHiddenWallTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TriggerMode TriggerMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AHiddenWall*> ConnectedWalls;

private:

	UPROPERTY()
	UBoxComponent* Collider;

	UPROPERTY()
	bool bIsFirstCollision = true;

	UPROPERTY()
	bool bIsPlayerInsideTrigger = false;

	UPROPERTY()
	bool bLookedAtPrimaryWall;

	UPROPERTY()
	AActor* PlayerActor;

	UFUNCTION()
	void ToggleConnectedWalls();

	UFUNCTION()
	void OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
					 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
					 int32 OtherBodyIndex);
	
};

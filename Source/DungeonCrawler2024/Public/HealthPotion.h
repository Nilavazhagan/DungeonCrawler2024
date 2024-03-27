// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupComponent.h"
#include "GameFramework/Actor.h"
#include "HealthPotion.generated.h"

UCLASS()
class DUNGEONCRAWLER2024_API AHealthPotion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int HealAmount = 20;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	UPickupComponent* PickupComponent;
	
	UFUNCTION()
	void OnPickup(const AActor* Actor);
};

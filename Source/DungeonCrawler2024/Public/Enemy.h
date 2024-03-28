// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "DamageComponent.h"
#include "TileBlockingComponent.h"
#include "Enemy.generated.h"


UCLASS()
class DUNGEONCRAWLER2024_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere)
	UHealthComponent* Health;
	UPROPERTY(EditAnywhere)
	UDamageComponent* DamageComponent;
	UPROPERTY(EditAnywhere)
	UTileBlockingComponent* TileBlockingComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

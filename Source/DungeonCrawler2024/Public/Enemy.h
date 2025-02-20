// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "DamageComponent.h"
#include "TickActorInterface.h"
#include "TileBlockingComponent.h"
#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
#include "DungeonCrawler2024/GridManager.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UCLASS()
class DUNGEONCRAWLER2024_API AEnemy : public ACharacter, public ITickActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDamageComponent* DamageComponent;
	UPROPERTY(EditAnywhere)
	UTileBlockingComponent* TileBlockingComponent;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* EquippedWeaponHolder;
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolTargets;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	UPROPERTY()
	AGridManager* GridManager;
	

	// Range within which player is visible to the enemy (unit: number of tiles)
	const int RANGE = 5;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)	
	void OnAttack(AActor* Target);

	virtual void OnEnemyTick() override;

	virtual AGridManager* GetGridManager() override
	{
		return GridManager;
	}
	virtual void CallPlayerTickDelayed() override
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AEnemy::OnPlayerTick);
	}
	virtual void CallEnemyTickDelayed() override
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AEnemy::OnEnemyTick);
	}
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDamageReceived(int HealthAfterDamage, AActor* Attacker);

private:

	UPROPERTY()
	float GridTileSize;
	
	UPROPERTY()
	ADungeonCrawler2024GameMode* GameMode;
	
	UPROPERTY()
	AActor* Player;

	UPROPERTY()
	AActor* CurrentTarget;

	UPROPERTY()
	FOnTickComplete OnTickComplete;

	UFUNCTION()
	TArray<FGridTileStruct> GetAdjacentTiles();

	UFUNCTION()
	void Move();

	UFUNCTION()
	void RotateTowards(const FVector& LookAt);

	UFUNCTION()
	void AttackPlayer();

	UFUNCTION()
	bool IsPlayerInRange();

	UFUNCTION()
	int GetPlayerDistance();

	UFUNCTION()
	AActor* GetClosestPatrolTarget();
	
	UFUNCTION()
	void UpdateTarget();
};

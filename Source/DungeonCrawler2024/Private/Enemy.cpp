// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->InitialHealth = 3;

	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

	DamageComponent->Damage = 1;

	TileBlockingComponent = CreateDefaultSubobject<UTileBlockingComponent>(TEXT("TileBlockingComponent"));


	EquippedWeaponHolder = CreateDefaultSubobject<UChildActorComponent>(TEXT("EquippedWeaponHolder"));
	EquippedWeaponHolder->SetupAttachment(this->GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health->RegisterComponent();
	DamageComponent->RegisterComponent();
	TileBlockingComponent->RegisterComponent();

	GameMode = Cast<ADungeonCrawler2024GameMode, AGameModeBase>(
		UGameplayStatics::GetGameMode(this->GetWorld()));
	Player = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	GridManager = GameMode->GridManager;
	GridTileSize = GridManager->GridTileSize;

	if (PatrolTargets.Num() > 0)
		CurrentTarget = PatrolTargets[0];
}

void AEnemy::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::OnEnemyTick()
{
	UpdateTarget(); 

	if (GetPlayerDistance() == 1)
		AttackPlayer();
	else
		Move();
}

TArray<FGridTileStruct> AEnemy::GetAdjacentTiles()
{
	FVector MyLocation = GetActorLocation();
	TArray<FGridTileStruct> AdjacentTiles = TArray<FGridTileStruct>();

	AdjacentTiles.Add(GridManager->GetAdjacentTileInDirection(MyLocation, GetActorForwardVector()));
	AdjacentTiles.Add(GridManager->GetAdjacentTileInDirection(MyLocation, -1 * GetActorForwardVector()));
	AdjacentTiles.Add(GridManager->GetAdjacentTileInDirection(MyLocation, -1 * GetActorRightVector()));
	AdjacentTiles.Add(GridManager->GetAdjacentTileInDirection(MyLocation, GetActorRightVector()));

	return AdjacentTiles;
}

void AEnemy::Move()
{
	if (CurrentTarget == nullptr)
	{
		ITickActorInterface::BroadcastTickComplete();
		return;
	}

	TArray<FGridTileStruct> AdjacentTiles = GetAdjacentTiles();

	float MinDistance = TNumericLimits<float>::Max();
	int NextTileIndex = 0;
	for(int i = 0, count = AdjacentTiles.Num(); i < count; i++)
	{
		FGridTileStruct Tile = AdjacentTiles[i];
		if (GridManager->IsTileBlocking(Tile))
			continue;

		const float DistanceToTarget = FVector::Dist(Tile.Position, CurrentTarget->GetActorLocation());
		if(DistanceToTarget < MinDistance)
		{
			MinDistance = DistanceToTarget;
			NextTileIndex = i;
		}
	}

	// If enemy is blocked on all 4 sides, it will fallback to index = 0, since that's the default value
	// This is fine for now, as it should be a very rare occurence.
	const FGridTileStruct NextTile = AdjacentTiles[NextTileIndex];
	RotateTowards(NextTile.Position);
	// Because AdjacentTiles is full of copies, use the copy's position data to get a reference to the original from the GridManager.
	FGridTileStruct& NextTileOriginal = GridManager->GetClosestTile(AdjacentTiles[NextTileIndex].Position);
	GridManager->MoveActorTo(this, NextTileOriginal);
	ITickActorInterface::BroadcastTickComplete();
}

void AEnemy::RotateTowards(const FVector& LookAt)
{
	const FVector MyLocation = GetActorLocation();
	
	// Using MyLocation.Z cos, enemy can only rotate in Z axis. Cannot look up/down
	const FVector TargetLocation = FVector(LookAt.X, LookAt.Y, MyLocation.Z);

	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
	SetActorRotation(NewRotation);
}

void AEnemy::AttackPlayer()
{
	RotateTowards(Player->GetActorLocation());
	OnAttack(Player);
}

bool AEnemy::IsPlayerInRange()
{
	return GetPlayerDistance() <= RANGE;
}

int AEnemy::GetPlayerDistance()
{
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MyLocation = GetActorLocation();

	const float Distance = FVector::Dist(MyLocation, PlayerLocation);
	return (int) UKismetMathLibrary::FCeil(Distance/GridTileSize);
}

AActor* AEnemy::GetClosestPatrolTarget()
{
	if (PatrolTargets.Num() <= 0)
		return nullptr;
	
	float MinDistance = TNumericLimits<float>::Max();
	int MinTargetIndex = 0;
	for (int i = 0, count = PatrolTargets.Num(); i < count; i++)
	{
		const AActor* Target = PatrolTargets[i];
		const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			MinTargetIndex = i;
		}
	}

	return PatrolTargets[MinTargetIndex];
}

void AEnemy::UpdateTarget()
{
	if (IsPlayerInRange())
		CurrentTarget = Player;
	else if (CurrentTarget == Player)
	{
		// When switching from Player-chasing to patrolling mode, just choose the closest patrol target
		CurrentTarget = GetClosestPatrolTarget();
	}

	if (CurrentTarget == nullptr)
		return;

	const FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();
	const FVector MyLocation = GetActorLocation();

	// Cycle between patrol targets - Z position doesn't matter
	if (CurrentTargetLocation.X == MyLocation.X &&
		CurrentTargetLocation.Y == MyLocation.Y)
	{
		int CurrentPatrolTargetIndex = 0;
		for (const int count = PatrolTargets.Num(); CurrentPatrolTargetIndex < count; CurrentPatrolTargetIndex++)
			if (CurrentTarget == PatrolTargets[CurrentPatrolTargetIndex])
				break;

		const int NextTargetIndex = (CurrentPatrolTargetIndex + 1) % PatrolTargets.Num();
		CurrentTarget = PatrolTargets[NextTargetIndex];
	}
}
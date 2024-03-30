// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenWallTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AHiddenWallTrigger::AHiddenWallTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AHiddenWallTrigger::BeginPlay()
{
	Super::BeginPlay();

	Collider = FindComponentByClass<UBoxComponent>();
	check(Collider != nullptr);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AHiddenWallTrigger::OnCollisionEnter);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AHiddenWallTrigger::OnCollisionExit);

	PlayerActor = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
}

// Called every frame
void AHiddenWallTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayerInsideTrigger && TriggerMode == TriggerMode::LookAway)
	{
		const FVector PlayerLocation = PlayerActor->GetActorLocation();
		FVector PlayerForwardDirection = PlayerActor->GetActorForwardVector();
		PlayerForwardDirection.Normalize();

		const FVector PrimaryWallLocation = ConnectedWalls[0]->GetActorLocation();
		const FVector PrimaryWallLocationAdjusted = FVector(PrimaryWallLocation.X, PrimaryWallLocation.Y, PlayerLocation.Z);
		FVector PrimaryWallDirection = PrimaryWallLocationAdjusted - PlayerLocation;
		PrimaryWallDirection.Normalize();
		UE_LOG(LogTemp, Error, TEXT("Facing PrimaryWallDirection? %hs"), (PrimaryWallDirection.Equals(PlayerForwardDirection) ? "true" : "false"));

		if (bLookedAtPrimaryWall == false)
		{
			if (PrimaryWallDirection.Equals(PlayerForwardDirection))
				bLookedAtPrimaryWall = true;
		}
		else
		{
			if (!PrimaryWallDirection.Equals(PlayerForwardDirection))
			{
				ToggleConnectedWalls();
				bLookedAtPrimaryWall = false;
			}
		}
	}
}

void AHiddenWallTrigger::ToggleConnectedWalls()
{
	for (AHiddenWall* Wall : ConnectedWalls)
		Wall->Toggle();
}

void AHiddenWallTrigger::OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor != PlayerActor)
		return;

	if (ConnectedWalls.Num() <= 0)
		return;

	switch (TriggerMode)
	{
	case TriggerMode::Once:
		if (!bIsFirstCollision)
			break;
		bIsFirstCollision = false;
	case TriggerMode::Everytime:
		ToggleConnectedWalls();
		break;
	case TriggerMode::LookAway:
		bIsPlayerInsideTrigger = true;
		break;
	}
}

void AHiddenWallTrigger::OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	bIsPlayerInsideTrigger = false;
}


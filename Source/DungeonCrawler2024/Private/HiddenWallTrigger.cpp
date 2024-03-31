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

	if (bIsPlayerInsideTrigger && (TriggerMode == TriggerMode::LookAway || TriggerMode == TriggerMode::OnceAndLookAway))
	{
		const FVector PlayerLocation = PlayerActor->GetActorLocation();
		FVector PlayerForwardDirection = PlayerActor->GetActorForwardVector();
		PlayerForwardDirection.Normalize();

		const FVector PrimaryWallLocation = ConnectedWalls[0]->GetActorLocation();
		const FVector PrimaryWallLocationAdjusted = FVector(PrimaryWallLocation.X, PrimaryWallLocation.Y, PlayerLocation.Z);
		FVector PrimaryWallDirection = PrimaryWallLocationAdjusted - PlayerLocation;
		PrimaryWallDirection.Normalize();


		float WallPlayerDotProduct = FVector::DotProduct(PlayerForwardDirection, PrimaryWallDirection);
		UE_LOG(LogTemp, Warning, TEXT("The dot product is %f"), WallPlayerDotProduct);

		if (bLookedAtPrimaryWall == false)
		{
			if (WallPlayerDotProduct > 0.5)
			{
				bLookedAtPrimaryWall = true;
			}
		}
		else
		{
			// If the dot product is less than 0, the player is looking more than 90 degrees away from the wall 
			if (WallPlayerDotProduct <= 0.01 || WallPlayerDotProduct == -0.0)
			{
				ToggleConnectedWalls();
				bLookedAtPrimaryWall = false;
				bIsFirstCollision = false;
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
	case TriggerMode::OnceAndLookAway:
		if (!bIsFirstCollision)
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


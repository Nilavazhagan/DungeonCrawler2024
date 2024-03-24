// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCrawler2024/Public/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "DungeonCrawler2024/GridManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> gridManagers = TArray<AActor*>(); 
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), TSubclassOf<AGridManager>(), gridManagers);

	GridManager = Cast<AGridManager, AActor>(gridManagers[0]);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
	EnhancedInputComponent->BindAction(TurnLeftInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnLeft);
	EnhancedInputComponent->BindAction(TurnRightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnRight);
}

void APlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	const FVector2d moveDirection = Value.Get<FVector2d>();

	if (moveDirection.X > 0.05)
	{
		// Forward
		GridManager->MoveActor(this, GetActorForwardVector());
	}
	else if (moveDirection. X < -0.05)
	{
		// Backward
		GridManager->MoveActor(this, -1 * GetActorForwardVector());
	}
	else if (moveDirection.Y < -0.05)
	{
		// Left
		GridManager->MoveActor(this, -1 * GetActorRightVector());
	}
	else if (moveDirection.Y > 0.05)
	{
		// Right
		GridManager->MoveActor(this, GetActorRightVector());
	}
}

void APlayerCharacter::TurnRight()
{
	AddActorLocalRotation(FRotator(0, 90, 0));
}

void APlayerCharacter::TurnLeft()
{
	AddActorLocalRotation(FRotator(0, -90, 0));
}



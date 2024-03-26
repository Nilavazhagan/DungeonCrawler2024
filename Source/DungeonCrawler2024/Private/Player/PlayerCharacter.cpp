// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCrawler2024/Public/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
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

	const ADungeonCrawler2024GameMode* GameMode = Cast<ADungeonCrawler2024GameMode, AGameModeBase>(
		UGameplayStatics::GetGameMode(this->GetWorld()));
	GridManager = GameMode->GridManager;
	GridManager->RegisterActor(this);
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

	EnhancedInputComponent->BindAction(MoveForwardInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
	EnhancedInputComponent->BindAction(MoveBackwardInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveBackward);
	EnhancedInputComponent->BindAction(MoveLeftInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveLeft);
	EnhancedInputComponent->BindAction(MoveRightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	EnhancedInputComponent->BindAction(TurnLeftInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnLeft);
	EnhancedInputComponent->BindAction(TurnRightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnRight);
}

void APlayerCharacter::MoveForward()
{
	GridManager->MoveActor(this, GetActorForwardVector());
}

void APlayerCharacter::MoveBackward()
{
	GridManager->MoveActor(this, -1 * GetActorForwardVector());
}

void APlayerCharacter::MoveLeft()
{
	GridManager->MoveActor(this, -1 * GetActorRightVector());
}

void APlayerCharacter::MoveRight()
{
	GridManager->MoveActor(this, GetActorRightVector());
}

void APlayerCharacter::TurnRight()
{
	AddActorLocalRotation(FRotator(0, 90, 0));
}

void APlayerCharacter::TurnLeft()
{
	AddActorLocalRotation(FRotator(0, -90, 0));
}



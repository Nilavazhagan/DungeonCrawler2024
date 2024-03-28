// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCrawler2024/Public/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
#include "DungeonCrawler2024/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interact.h"
#include "Attack.h"
#include "HealthComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	EquippedWeaponHolder = CreateDefaultSubobject<UChildActorComponent>(TEXT("EquippedWeaponHolder"));
	EquippedWeaponHolder->SetupAttachment(this->GetRootComponent());

	TorchHolder = CreateDefaultSubobject<UChildActorComponent>(TEXT("TorchHolder"));
	TorchHolder->SetupAttachment(this->GetRootComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	const ADungeonCrawler2024GameMode* GameMode = Cast<ADungeonCrawler2024GameMode, AGameModeBase>(
		UGameplayStatics::GetGameMode(this->GetWorld()));
	GridManager = GameMode->GridManager;
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

	// Binding Inputs
	EnhancedInputComponent->BindAction(MoveForwardInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
	EnhancedInputComponent->BindAction(MoveBackwardInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveBackward);
	EnhancedInputComponent->BindAction(MoveLeftInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveLeft);
	EnhancedInputComponent->BindAction(MoveRightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	EnhancedInputComponent->BindAction(TurnLeftInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnLeft);
	EnhancedInputComponent->BindAction(TurnRightInput, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnRight);
	EnhancedInputComponent->BindAction(InteractInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
	EnhancedInputComponent->BindAction(AttackInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
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

// Upon pressing the interact key, triggers the interact interface on any actors
// occupying the tile directly in front.
void APlayerCharacter::Interact()
{
	UE_LOG(LogTemp, Display, TEXT("Interact input received!"));

	// Getting the tile data to check
	const FVector CheckLocation = FVector(GetActorLocation() + (this->GetActorForwardVector() * GridManager->GridTileSize));
	FGridTileStruct Tile;
	if (!GetForwardTile(Tile))
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Interact: checking actors on tile location (%f, %f, %f)!"), Tile.Position.X, Tile.Position.Y, Tile.Position.Z);

	// Checking each actor
	for (AActor* OccupyingActor : Tile.ActorsOccupying)
	{
		UE_LOG(LogTemp, Display, TEXT("Attempting to interact."));
		if (OccupyingActor->Implements<UInteract>())
		{
			IInteract::Execute_OnInteract(OccupyingActor, this);
		}
	}
}

void APlayerCharacter::Attack()
{
	UChildActorComponent* ActiveWeaponHolder;
	if (!EquippedWeaponHolder->GetChildActor())
	{
		ActiveWeaponHolder = TorchHolder;
	}
	else
	{
		ActiveWeaponHolder = EquippedWeaponHolder;
	}

	// Get the tile to attack
	FGridTileStruct Tile;

	// If bad forward tile, do not attack
	if (!GetForwardTile(Tile))
	{
		return;
	}

	// Identify a valid target actor
	AActor* Target{};
	
	for (AActor* OccupyingActor : Tile.ActorsOccupying)
	{
		FString ActorName = OccupyingActor->GetActorNameOrLabel();
		if (OccupyingActor->GetComponentByClass<UHealthComponent>())
		{
			Target = OccupyingActor;
			break;
		}
	}

	// Access the weapon and perform its attack if it has one
	if (ActiveWeaponHolder->GetChildActor()->Implements<UAttack>())
	{
		IAttack::Execute_OnAttack(ActiveWeaponHolder->GetChildActor(), this, Target);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon DOES NOT implement the attack interface!"))
	}
}

// Returns a reference to the tile directly in front of the player, returns false if forward tile doesn't exist
bool APlayerCharacter::GetForwardTile(FGridTileStruct& Tile)
{
	const FVector CheckLocation = FVector(GetActorLocation() + (this->GetActorForwardVector() * GridManager->GridTileSize));
	Tile = GridManager->GetClosestTile(CheckLocation);
	if (Tile.Position == GridManager->GetClosestTile(this->GetActorLocation()).Position)
	{
		return false;
	}
	else
	{
		return true;
	}
}
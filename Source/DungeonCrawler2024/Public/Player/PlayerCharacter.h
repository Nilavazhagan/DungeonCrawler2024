// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponent.h"
#include "InputAction.h"
#include "DungeonCrawler2024/DungeonCrawler2024Character.h"
#include "DungeonCrawler2024/GridManager.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DUNGEONCRAWLER2024_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Due to what seems to be a bug with Unreal. To access the defaults for this property, drag it
	// into the blueprint event graph like a variable, and it is editable there, even if not attached
	// to anything.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* EquippedWeaponHolder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward();

	void MoveBackward();

	void MoveLeft();

	void MoveRight();

	void TurnRight();

	void TurnLeft();

	void Interact();

	void Attack();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveBackwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveLeftInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveRightInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* TurnLeftInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* TurnRightInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackInput;



private:

	AGridManager* GridManager;

	// Gets the tile directly in front of the player
	bool GetForwardTile(FGridTileStruct& Tile);
};

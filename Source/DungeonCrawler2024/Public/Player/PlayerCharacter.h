// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeysInventoryComponent.h"
#include "InputAction.h"
#include "TickActorInterface.h"
#include "DungeonCrawler2024/DungeonCrawler2024Character.h"
#include "DungeonCrawler2024/DungeonCrawler2024GameMode.h"
#include "DungeonCrawler2024/GridManager.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DUNGEONCRAWLER2024_API APlayerCharacter : public ACharacter, public ITickActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// The holders for the player's torch weapon and any weapon they find along the way
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* EquippedWeaponHolder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* TorchHolder;
	
	// The Player's keyinventory component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UKeysInventoryComponent* KeyInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundCue*> FootstepSounds;
	

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

	UFUNCTION(BlueprintCallable)
	void TickIfTrue(bool Check = true);

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

	UFUNCTION()
	void Equip(AActor* Weapon) const;

	virtual void OnPlayerTick() override;

	UFUNCTION(BlueprintCallable)
	void SetShouldBlockInput(bool ShouldBlockInput);

	UFUNCTION(BlueprintCallable)
	bool ShouldBlockInput();

	UFUNCTION(BlueprintCallable)
	FString GetActiveWeaponName();

	virtual AGridManager* GetGridManager() override
	{
		return GridManager;
	}
	virtual void CallPlayerTickDelayed() override
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &APlayerCharacter::OnPlayerTick);
	}
	virtual void CallEnemyTickDelayed() override
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &APlayerCharacter::OnEnemyTick);
	}

private:

	UPROPERTY()
	ADungeonCrawler2024GameMode* GameMode;
	UPROPERTY()
	AGridManager* GridManager;
	UPROPERTY()
	bool bShouldBlockInput;
	UPROPERTY()
	FOnTickComplete OnTickComplete;

	void PlayFootstepSound();

	// Gets the tile directly in front of the player
	bool GetForwardTile(FGridTileStruct& Tile);
};

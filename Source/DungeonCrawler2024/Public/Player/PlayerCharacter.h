// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward();

	void MoveBackward();

	void MoveLeft();

	void MoveRight();

	void TurnRight();

	void TurnLeft();

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

private:

	AGridManager* GridManager;
};

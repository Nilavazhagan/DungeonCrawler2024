// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONCRAWLER2024_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InitialHealth;

	UPROPERTY(VisibleInstanceOnly)
	int CurrentHealth;

public:

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int damage);

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmt);

private:
	
	UFUNCTION()
	void Die();

	const int Min_Health{ 0 };

	UPROPERTY(EditAnywhere)
	int MaxHealth{ 10 };
};

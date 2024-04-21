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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InitialHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth{ 10 };

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FDamageReceivedDelegate, int, HealthAfterDamage, AActor*, Attacker);
	UPROPERTY(BlueprintReadWrite)
	FDamageReceivedDelegate OnDamaged;

	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_DELEGATE(FOnDeathDelegate);
	UPROPERTY(BlueprintReadWrite)
	FOnDeathDelegate OnDeath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;





public:

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int damage, AActor* Attacker);

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmt);

private:
	
	UFUNCTION(BlueprintCallable)
	void Die();

	const int Min_Health{ 0 };


};

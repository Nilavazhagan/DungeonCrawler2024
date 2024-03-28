// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONCRAWLER2024_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageComponent();

protected:


public:
	UFUNCTION(BlueprintCallable)
	void DealDamageTo(AActor* Target);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;
};

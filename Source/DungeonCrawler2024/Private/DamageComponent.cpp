// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "HealthComponent.h"


// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Damage = 1;
}

// Handles dealing damage to a target actor
void UDamageComponent::DealDamageTo(AActor* Target)
{
	UHealthComponent* TargetHealth = Target->GetComponentByClass<UHealthComponent>();
	if (!TargetHealth) return;
	TargetHealth->ReceiveDamage(Damage);
	return;
}


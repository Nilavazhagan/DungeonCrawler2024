// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InitialHealth = MaxHealth;
	CurrentHealth = InitialHealth;
	this->RegisterComponent();
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = InitialHealth;
}

void UHealthComponent::ReceiveDamage(int Damage)
{
	CurrentHealth -= Damage;
	if (CurrentHealth < Min_Health)
		Die();
}

void UHealthComponent::Heal(int HealAmt)
{
	CurrentHealth += HealAmt;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}

void UHealthComponent::Die()
{
	GetOwner()->Destroy();
}


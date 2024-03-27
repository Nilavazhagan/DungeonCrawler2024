// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = InitialHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ReceiveDamage(int damage)
{
	CurrentHealth -= damage;
	if (CurrentHealth < Min_Health)
		Die();
}

void UHealthComponent::Heal(int healAmt)
{
	CurrentHealth += healAmt;
	if (CurrentHealth > Max_Health)
		CurrentHealth = Max_Health;
}

void UHealthComponent::Die()
{
	GetOwner()->Destroy();
}


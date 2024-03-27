// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"

#include "HealthComponent.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AHealthPotion::AHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	PickupComponent = Cast<UPickupComponent>(GetComponentByClass(UPickupComponent::StaticClass()));
	check(PickupComponent != nullptr);
	PickupComponent->OnPickup.BindDynamic(this, &AHealthPotion::OnPickup);
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthPotion::OnPickup(const AActor* Actor) 
{
	UHealthComponent* HealthComponent = Cast<UHealthComponent>(
		Actor->GetComponentByClass(UHealthComponent::StaticClass()));
	check(HealthComponent != nullptr);

	HealthComponent->Heal(HealAmount);
}

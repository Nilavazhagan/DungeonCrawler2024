// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "HealthPotion.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	PickupComponent = Cast<UPickupComponent>(GetComponentByClass(UPickupComponent::StaticClass()));
	if (PickupComponent == nullptr)
		return;
	PickupComponent->OnPickup.BindDynamic(this, &AWeapon::OnPickup);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnPickup(const AActor* Actor) 
{
	const APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(Actor);
	if (PlayerChar == nullptr)
		return;
	PlayerChar->Equip(this);
}
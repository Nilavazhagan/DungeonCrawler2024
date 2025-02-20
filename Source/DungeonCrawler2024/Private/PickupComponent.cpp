// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "Components/ShapeComponent.h"
#include "Player/PlayerCharacter.h"


// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	bDoesBlockTile = false;
	
	Collider = GetOwner()->FindComponentByClass<UShapeComponent>();
	check(Collider != nullptr)
	Collider->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnCollision);
}


void UPickupComponent::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("UPickupComponent OnCollision Called!"));

	// Check if the other actor is the player character.
	const APlayerCharacter* playerChar = Cast<APlayerCharacter>(OtherActor);
	if (playerChar == nullptr)
		return;

	// Call on pickup on the actor that owns this component.
	OnPickup.ExecuteIfBound(playerChar);

	if (bDestroyOnCollision)
		GetOwner()->Destroy();
}


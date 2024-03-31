// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBlockingComponent.h"
#include "PickupComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONCRAWLER2024_API UPickupComponent : public UTileBlockingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPickupComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (DisplayName= "Destroy On Collision"))
	bool bDestroyOnCollision = true;

public:
	UDELEGATE(BlueprintCallable)
	DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPickupDelegate, const AActor*, Actor);

	UPROPERTY(BlueprintReadWrite)
	FOnPickupDelegate OnPickup;

	UPROPERTY(EditAnywhere, NoClear)
	UShapeComponent* Collider;

private:

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeysInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRAWLER2024_API UKeysInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeysInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> HeldKeyIDs{};

protected:


public:	

	UFUNCTION(BlueprintCallable)
	void AddKeyID(int KeyID);
	UFUNCTION(BlueprintCallable)
	void RemoveKeyID(int KeyID);
	UFUNCTION(BlueprintCallable)
	bool GetHasKeyID(int KeyID);

		
};

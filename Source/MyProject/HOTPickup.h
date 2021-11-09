// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HOTPickup.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AHOTPickup : public APickup
{
	GENERATED_BODY()

public:

	void OnPickup(AActor* ActorThatPickedUp) override;

	UFUNCTION(BlueprintCallable)
		void OnGenerate() override;
};
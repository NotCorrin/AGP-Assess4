// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TripleShotPickup.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API ATripleShotPickup : public APickup
{
	GENERATED_BODY()

		void OnPickup(AActor* ActorThatPickedUp) override;

	UFUNCTION(BlueprintCallable)
		void OnGenerate() override;
};

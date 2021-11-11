// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "InstantHealthPickup.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AInstantHealthPickup : public APickup
{
	GENERATED_BODY()

public:

	void OnPickup(AActor* ActorThatPickedUp) override;

};


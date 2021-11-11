// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "IncreasedDamagePickup.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AIncreasedDamagePickup : public APickup
{
	GENERATED_BODY()

public:

	void OnPickup(AActor* ActorThatPickedUp) override;

};

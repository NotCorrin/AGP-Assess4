// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "LightArmorPickup.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API ALightArmorPickup : public APickup
{
	GENERATED_BODY()

	void OnPickup(AActor* ActorThatPickedUp) override;

	UFUNCTION(BlueprintCallable)
	void OnGenerate() override;

};

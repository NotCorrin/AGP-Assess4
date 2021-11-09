// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PickupManager.h"
#include "TimerManager.h"
#include "MultiplayerGameMode.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	TArray<class APickupNode*> AllPickupNodes;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APickup> SprintPickupClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APickup> JumpPickupClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APickup> InstantHealthPickupClass;

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages) override;


private:

	APickupManager* PickupManager;
};

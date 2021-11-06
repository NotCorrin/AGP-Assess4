// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupManager.generated.h"

UCLASS()
class MYPROJECT_API APickupManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Pickups")
		int32 NumPickups;
	UPROPERTY(EditAnywhere, Category = "Pickups")
		TSubclassOf<class APickup> PickupsToSpawn;
	UPROPERTY(EditAnywhere, Category = "Pickup Nodes")
		TArray<class APickupNode*> AllPickupNodes;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PopulateNodes();
	void CreateAgents();
};

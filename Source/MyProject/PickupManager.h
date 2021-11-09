// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
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
		TSubclassOf<class APickup> SprintPickup;

	UPROPERTY(EditAnywhere, Category = "Pickups")
		TSubclassOf<class APickup> JumpPickup;

	UPROPERTY(EditAnywhere, Category = "Pickups")
		TSubclassOf<class APickup> InstantHealthPickupClass;

	UPROPERTY(EditAnywhere, Category = "Pickup Nodes")
		TArray<class APickupNode*> AllPickupNodes;

	//UPROPERTY(Replicated)
	int32 PickupNodeIndex;

	//UPROPERTY(Replicated)
	int32 RandPickup;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PopulateNodes();

	void CreateAgents();

	void Init(const TArray<class APickupNode*>& PossibleSpawnLocationsArg, TSubclassOf<class APickup> PickupClassArg, float SpawnIntervalArg);

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	TArray<APickupNode*> PossibleSpawnLocations;
	TSubclassOf<APickup> PickupClass;
	float SpawnInterval;

	const float PICKUP_LIFETIME = 20.0f;
	FTimerHandle SpawnTimer;

	void SpawnPickup();
};

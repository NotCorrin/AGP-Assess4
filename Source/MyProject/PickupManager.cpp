// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "EngineUtils.h"
#include "PickupNode.h"
#include "Engine/GameEngine.h"
#include "Net/UnrealNetwork.h"
#include "Pickup.h"
#include "InstantHealthPickup.h"

// Sets default values
APickupManager::APickupManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupManager::BeginPlay()
{
	Super::BeginPlay();

	if (AllPickupNodes.Num() == 0)
	{
		PopulateNodes();
	}

	//CreateAgents();

	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &APickupManager::SpawnPickup, SpawnInterval, true, 0.0f);

}

// Called every frame
void APickupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupManager::PopulateNodes()
{
	for (TActorIterator<APickupNode> It(GetWorld()); It; ++It)
	{
		AllPickupNodes.Add(*It);	//Adds all pick up nodes in the level to an array
	}
}

void APickupManager::CreateAgents()
{
	if (AllPickupNodes.Num() > 0)	//checks if there are pick up nodes in the level
	{
		for (int32 i = 0; i < NumPickups; i++)	//iterates through the specified number of power ups
		{

			PickupNodeIndex = FMath::RandRange(0, AllPickupNodes.Num() - 1);	//selects a random navigation node

			RandPickup = FMath::RandRange(0, 100);

			if (RandPickup < 10)
			{
				APickup* Pickup = GetWorld()->SpawnActor<APickup>(SprintPickup, AllPickupNodes[PickupNodeIndex]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[PickupNodeIndex]->GetActorRotation());	//spawns the power up on the navigation node

				if (NumPickups > 1)	//checks if there is more than 1 specified number of power ups
				{
					AllPickupNodes.RemoveAt(PickupNodeIndex);	//removes the pick up node from the array
				}

				UE_LOG(LogTemp, Warning, TEXT("SprintPickup"));

			}
			else if (RandPickup >= 10)
			{
				APickup* Pickup = GetWorld()->SpawnActor<APickup>(JumpPickup, AllPickupNodes[PickupNodeIndex]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[PickupNodeIndex]->GetActorRotation());	//spawns the power up on the navigation node

				if (NumPickups > 1)	//checks if there is more than 1 specified number of power ups
				{
					AllPickupNodes.RemoveAt(PickupNodeIndex);	//removes the pick up node from the array
				}

				UE_LOG(LogTemp, Warning, TEXT("JumpPickup"));
			}




			UE_LOG(LogTemp, Warning, TEXT("Pickup Node Length: %i"), AllPickupNodes.Num());
		}
	}
}

/*void APickupManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupManager, PickupNodeIndex);
	DOREPLIFETIME(APickupManager, RandPickup);

}*/

void APickupManager::Init(const TArray<class APickupNode*>& PossibleSpawnLocationsArg, TSubclassOf<class APickup> PickupClassArg, float SpawnIntervalArg)
{
	PossibleSpawnLocations = PossibleSpawnLocationsArg;
	PickupClass = PickupClassArg;
	SpawnInterval = SpawnIntervalArg;
}

void APickupManager::SpawnPickup()
{
	int32 RandomLocation = FMath::RandRange(0, PossibleSpawnLocations.Num() - 1);
	AInstantHealthPickup* Pickup = GetWorld()->SpawnActor<AInstantHealthPickup>(InstantHealthPickupClass, PossibleSpawnLocations[RandomLocation]->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f), FRotator::ZeroRotator);
	Pickup->SetLifeSpan(PICKUP_LIFETIME);
}
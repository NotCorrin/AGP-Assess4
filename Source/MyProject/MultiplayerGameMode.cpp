// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "PickupNode.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "InstantHealthPickup.h"
#include "SprintPickup.h"
#include "JumpPickup.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);

	for (TActorIterator<APickupNode> It(GetWorld()); It; ++It)
	{
		AllPickupNodes.Add(*It);	//Adds all pick up nodes in the level to an array
	}

	UWorld* World = GetWorld();

	if (AllPickupNodes.Num() > 0) {

		int32 NumPickupSpawn = 1;

		for (int32 i = 0; i < NumPickupSpawn; i++)
		{
			int32 RandPickupNode = FMath::RandRange(0, AllPickupNodes.Num() - 1);

			int32 WhichPowerup = FMath::RandRange(1, 30);
			int32 WhichMovementPowerup = FMath::RandRange(1, 100);
			int32 WhichHealthPowerup = FMath::RandRange(1, 100);

			/*if (WhichPowerup <= 15)
			{
				if (WhichMovementPowerup <= 50)
				{
					if (World)
					{
						ASprintPickup* Pickup = GetWorld()->SpawnActor<ASprintPickup>(SprintPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());

						if (AllPickupNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Spawned"));
					}
				}
				else
				{
					if (World)
					{
						AJumpPickup* Pickup = GetWorld()->SpawnActor<AJumpPickup>(JumpPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());

						if (AllPickupNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Spawned"));
					}
				}
			}
			else
			{
				if (World)
				{
					AInstantHealthPickup* Pickup = GetWorld()->SpawnActor<AInstantHealthPickup>(InstantHealthPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());

					if (AllPickupNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}

					UE_LOG(LogTemp, Warning, TEXT("Spawned"));
				}
			}*/

			AJumpPickup* Pickup = GetWorld()->SpawnActor<AJumpPickup>(JumpPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());

			/*if (AllPickupNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
			{
				AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
			}*/

			UE_LOG(LogTemp, Warning, TEXT("Spawned"));
		}

	}
}
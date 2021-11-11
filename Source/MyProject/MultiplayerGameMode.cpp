// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "PickupNode.h"
#include "NavigationNode.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "SprintPickup.h"
#include "JumpPickup.h"
#include "InstantHealthPickup.h"
#include "HOTPickup.h"
#include "LightArmorPickup.h"
#include "HeavyArmorPickup.h"
#include "Kismet/GameplayStatics.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);
	
	FTimerHandle TimerHandle;
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerGameMode::GeneratePickupFunctionality, 1, false);
}

void AMultiplayerGameMode::GeneratePickupFunctionality()
{
	for (TActorIterator<APickupNode> It(GetWorld()); It; ++It)
	{
		AllPickupNodes.Add(*It);	//Adds all pick up nodes in the level to an array
	}

	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		AllNavigationNodes.Add(*It);	//Adds all pick up nodes in the level to an array
	}

	if (AllNavigationNodes.Num() > 0) {

		int32 NumPickupSpawn = FMath::RandRange(1, AllNavigationNodes.Num());

		for (int32 i = 0; i < NumPickupSpawn; i++)
		{
			int32 RandPickupNode = FMath::RandRange(0, AllNavigationNodes.Num() - 1);

			int32 WhichPowerup = FMath::RandRange(1, 100);
			int32 WhichMovementPowerup = FMath::RandRange(1, 100);
			int32 WhichHealthPowerup = FMath::RandRange(1, 100);
			int32 WhichArmorPowerup = FMath::RandRange(1, 100);
			int32 WhichWeaponShotType = FMath::RandRange(1, 100);

			if (WhichPowerup <= 30)
			{
				if (WhichMovementPowerup <= 50)
				{
					ASprintPickup* Pickup = GetWorld()->SpawnActor<ASprintPickup>(SprintPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
				else
				{
					AJumpPickup* Pickup = GetWorld()->SpawnActor<AJumpPickup>(JumpPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
				
			}
			else if (WhichPowerup > 30 && WhichPowerup <= 70)
			{
				if (WhichHealthPowerup <= 70)
				{
					AInstantHealthPickup* Pickup = GetWorld()->SpawnActor<AInstantHealthPickup>(InstantHealthPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
				else
				{
					AHOTPickup* Pickup = GetWorld()->SpawnActor<AHOTPickup>(HOTPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
			}
			else
			{
				if (WhichArmorPowerup <= 60)
				{
					ALightArmorPickup* Pickup = GetWorld()->SpawnActor<ALightArmorPickup>(LightArmorPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
				else
				{
					AHeavyArmorPickup* Pickup = GetWorld()->SpawnActor<AHeavyArmorPickup>(HeavyArmorPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

					if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
					{
						AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
					}
				}
			}
		}
	}
}

void AMultiplayerGameMode::GameOver(AController* Controller)
{
	//UGameplayStatics::OpenLevel(this, "GameOver");
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "PickupNode.h"
#include "NavigationNode.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "SprintPickup.h"
#include "JumpPickup.h"
#include "InstantHealthPickup.h"
#include "HOTPickup.h"
#include "LightArmorPickup.h"
#include "HeavyArmorPickup.h"
#include "TripleShotPickup.h"
#include "ExplosivePickup.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);

	UE_LOG(LogTemp, Warning, TEXT("PLEAAASE"));
	
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

	UE_LOG(LogTemp, Warning, TEXT("LEZZ GOO"));

	UWorld* World = GetWorld();

	if (AllNavigationNodes.Num() > 0) {

		int32 NumPickupSpawn = FMath::RandRange(3, AllNavigationNodes.Num());

		for (int32 i = 0; i < NumPickupSpawn; i++)
		{
			int32 RandPickupNode = FMath::RandRange(0, AllNavigationNodes.Num() - 1);

			int32 WhichPowerup = FMath::RandRange(1, 100);
			int32 WhichMovementPowerup = FMath::RandRange(1, 100);
			int32 WhichHealthPowerup = FMath::RandRange(1, 100);
			int32 WhichArmorPowerup = FMath::RandRange(1, 100);
			int32 WhichWeaponShotType = FMath::RandRange(1, 100);

			if (WhichPowerup <= 15)
			{
				if (WhichMovementPowerup <= 50)
				{
					if (World)
					{
						ASprintPickup* Pickup = GetWorld()->SpawnActor<ASprintPickup>(SprintPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Sprint Spawned"));
					}
				}
				else
				{
					if (World)
					{
						AJumpPickup* Pickup = GetWorld()->SpawnActor<AJumpPickup>(JumpPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Jump Spawned"));
					}
				}
				
			}
			else if (WhichPowerup > 15 && WhichPowerup <= 40)
			{
				if (WhichHealthPowerup <= 70)
				{
					if (World)
					{
						AInstantHealthPickup* Pickup = GetWorld()->SpawnActor<AInstantHealthPickup>(InstantHealthPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Instant Health Spawned"));
					}
				}
				else
				{
					if (World)
					{
						AHOTPickup* Pickup = GetWorld()->SpawnActor<AHOTPickup>(HOTPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("HOT Spawned"));
					}
				}
			}
			else if (WhichPowerup > 40 && WhichPowerup <= 60)
			{
				if (WhichArmorPowerup <= 60)
				{
					if (World)
					{
						ALightArmorPickup* Pickup = GetWorld()->SpawnActor<ALightArmorPickup>(LightArmorPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Light Armor Spawned"));
					}
				}
				else
				{
					if (World)
					{
						AHeavyArmorPickup* Pickup = GetWorld()->SpawnActor<AHeavyArmorPickup>(HeavyArmorPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Heavy Armor Spawned"));
					}
				}
			}
			else
			{
				if (WhichWeaponShotType <= 45)
				{
					if (World)
					{
						ATripleShotPickup* Pickup = GetWorld()->SpawnActor<ATripleShotPickup>(TripleShotPickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Triple Shot Spawned"));
					}
				}
				else
				{
					if (World)
					{
						AExplosivePickup* Pickup = GetWorld()->SpawnActor<AExplosivePickup>(ExplosivePickupClass, AllNavigationNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllNavigationNodes[RandPickupNode]->GetActorRotation());

						if (AllNavigationNodes.Num() > 1)	//checks if there is more than 1 specified number of power ups
						{
							AllNavigationNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array
						}

						UE_LOG(LogTemp, Warning, TEXT("Triple Shot Spawned"));
					}
				}
			}
		}
	}

	/*if (AllPickupNodes.Num() > 0) {

		int32 NumPickupSpawn = 1;

		for (int32 i = 0; i < NumPickupSpawn; i++)
		{
			int32 RandPickupNode = FMath::RandRange(0, AllPickupNodes.Num() - 1);

			int32 WhichPowerup = FMath::RandRange(1, 30);
			int32 WhichMovementPowerup = FMath::RandRange(1, 100);
			int32 WhichHealthPowerup = FMath::RandRange(1, 100);

			if (WhichPowerup <= 15)
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
			}
		}
	}*/
}

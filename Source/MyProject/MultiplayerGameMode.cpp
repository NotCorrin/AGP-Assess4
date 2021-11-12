// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "PickupNode.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "SprintPickup.h"
#include "JumpPickup.h"
#include "InstantHealthPickup.h"
#include "HOTPickup.h"
#include "PlayerHUD.h"
#include "LightArmorPickup.h"
#include "HeavyArmorPickup.h"
#include "IncreasedDamagePickup.h"
#include "Kismet/GameplayStatics.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);
	
	FTimerHandle TimerHandle;
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerGameMode::GeneratePickupFunctionality, 1, false);	//waits for the rooms to finish generating before calling the pickups to spawn
}

void AMultiplayerGameMode::GeneratePickupFunctionality()
{
	for (TActorIterator<APickupNode> It(GetWorld()); It; ++It)
	{
		AllPickupNodes.Add(*It);	//Adds all pickup nodes in the world to an array
	}

	if (AllPickupNodes.Num() > 0) {

		int32 NumPickupSpawn = FMath::RandRange(1, AllPickupNodes.Num());	//picks a random number of pickups to spawn

		for (int32 i = 0; i < NumPickupSpawn; i++)
		{
			int32 RandPickupNode = FMath::RandRange(0, AllPickupNodes.Num() - 1);	//picks a random pickup node to spawn on

			int32 WhichPickup = FMath::RandRange(1, 100);	//picks a random pickup to spawn

			if (WhichPickup <= 10)
			{
				ASprintPickup* Pickup = GetWorld()->SpawnActor<ASprintPickup>(SprintPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spawns sprint pickup on random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else if (WhichPickup > 10 && WhichPickup <= 20)
			{
				AJumpPickup* Pickup = GetWorld()->SpawnActor<AJumpPickup>(JumpPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spawns jump pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else if (WhichPickup > 20 && WhichPickup <= 40)
			{
				AInstantHealthPickup* Pickup = GetWorld()->SpawnActor<AInstantHealthPickup>(InstantHealthPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());		//spawns instant health pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else if (WhichPickup > 40 && WhichPickup <= 55)
			{
				AHOTPickup* Pickup = GetWorld()->SpawnActor<AHOTPickup>(HOTPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spawns HOT pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else if (WhichPickup > 55 && WhichPickup <= 75)
			{
				ALightArmorPickup* Pickup = GetWorld()->SpawnActor<ALightArmorPickup>(LightArmorPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spwans light armor pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else if (WhichPickup > 75 && WhichPickup <= 90)
			{
				AHeavyArmorPickup* Pickup = GetWorld()->SpawnActor<AHeavyArmorPickup>(HeavyArmorPickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spawns heavy armor pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
			else
			{
				AIncreasedDamagePickup* Pickup = GetWorld()->SpawnActor<AIncreasedDamagePickup>(IncreasedDamagePickupClass, AllPickupNodes[RandPickupNode]->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f), AllPickupNodes[RandPickupNode]->GetActorRotation());	//spawns increased damage pickup on a random pickup node

				if (AllPickupNodes.Num() > 1)
				{
					AllPickupNodes.RemoveAt(RandPickupNode);	//removes the pick up node from the array to prevent multiple pickups spawning on the same node
				}
			}
		}
	}
}

void AMultiplayerGameMode::GameOver(AController* Controller)
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

		APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerController->GetPawn());

		if (Character)
		{
			Character->SetGameOver();	//sets HUD of the player character that died to show game over
		}
	}

	APawn* Pawn = Controller->GetPawn();
	if (Pawn)
	{
		Pawn->SetLifeSpan(0.1f);	//removes player character that died from the world
	}

	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		AllPlayerCharacters.Add(*It);	//finds all othe player characters in the world
	}

	if (AllPlayerCharacters.Num() > 0)
	{
		for (int32 i = 0; i < AllPlayerCharacters.Num(); i++)
		{
			AllPlayerCharacters[i]->SetGameOver();	//sets HUD of the other player character to show game over
			AllPlayerCharacters[i]->SetLifeSpan(0.1f);	//removes other player character from the world
		}
	}
}
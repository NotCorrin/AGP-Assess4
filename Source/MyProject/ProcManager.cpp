// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcManager.h"
#include "Engine/World.h"

// Sets default values
AProcManager::AProcManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set the default value of the maximum number of rooms that can spawn
	// These are just the "main rooms", not the "branching" rooms
	MaxRoomNumber = 8;
}

// Called when the game starts or when spawned
void AProcManager::BeginPlay()
{
	Super::BeginPlay();

	// Create a 1D array which will represent a 17x17 2D grid
	LevelMap.Init(false, 17*17);

	GenerateStartingRoom();
}

bool AProcManager::GetMapValue(int XIndex, int YIndex)
{
	// Return true or false based on the X and Y coordinates given to the LevelMap
	return LevelMap[YIndex * 17 + XIndex];
}


void AProcManager::SetMapValue(int XIndex, int YIndex)
{
	// Set true or false based on the X and Y coordinates in the LevelMap
	LevelMap[YIndex * 17 + XIndex] = true;
}

void AProcManager::GenerateStartingRoom_Implementation()
{
	// Create the starting room (which in turn, will generate more rooms)
	Rooms.Add(GetWorld()->SpawnActor<AProcRoom>(Room, FVector::ZeroVector, FRotator::ZeroRotator));

	// Set the default map value to be in the middle of the generated grid
	Rooms[0]->XCoord = 8;
	Rooms[0]->YCoord = 8;
	SetMapValue(8, 8);

	// Randomly select which starting orientation to pick
	const int StartingDoor = FMath::RandRange(0, 3);
	switch (StartingDoor)
	{
	case 0: Rooms[0]->bHasUpDoor = true; break;
	case 1: Rooms[0]->bHasDownDoor = true; break;
	case 2: Rooms[0]->bHasRightDoor = true; break;
	case 3: Rooms[0]->bHasLeftDoor = true; break;
	}
	// Continue setting parameters, followed by the function call which will start a chain reaction
	Rooms[0]->bIsStart = true;
	Rooms[0]->RoomNumber = 1;
	Rooms[0]->ProcManager = this;
	Rooms[0]->Node = GetWorld()->SpawnActor<ANavigationNode>(NodeClass, FVector(200, 200, 5), FRotator::ZeroRotator);
	Rooms[0]->GenerateWalls();
}
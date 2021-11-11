// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcRoom.h"

#include "ProcManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AProcRoom::AProcRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Component"));
	SetRootComponent(Floor);

	bCanGenUp = true;
	bCanGenDown = true;
	bCanGenRight = true;
	bCanGenLeft = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AProcRoom::BeginPlay()
{
	Super::BeginPlay();
}

void AProcRoom::GenerateWalls()
{
	// Spawn the Start Room Indicator
	
	if (bIsStart)
	{
		GetWorld()->SpawnActor<AActor>(StartClass, GetActorLocation() + FVector(0, 0, 800), FRotator::ZeroRotator);
	}

	// If Else chain follows the proceeding format:
	// 
	// 1. Check if the room should spawn a door pointing in a particular direction
	// 2. Spawn the Door Wall object
	// 3. Perform checks whether or not to generate an entire room after placing the Door Wall
	// 3. Generate the Next Room using parameters based on the current room
	
	if (bHasUpDoor)
	{
		GetWorld()->SpawnActor<AActor>(WallDoorClass, GetActorLocation() + FVector(400, 0, 0), FRotator(0, 90, 0));
		if (bCanGenUp && !ProcManager->GetMapValue(XCoord, YCoord + 1))
			GenerateNextRoom(GetActorLocation() + FVector(400, 0, 0), false, true, false, false);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(WallClass, GetActorLocation() + FVector(400, 0, 0), FRotator(0, 90, 0));
	}

	if (bHasDownDoor)
	{
		GetWorld()->SpawnActor<AActor>(WallDoorClass, GetActorLocation() + FVector(0, 0, 0), FRotator(0, 90, 0));
		if (bCanGenDown && !ProcManager->GetMapValue(XCoord, YCoord - 1))
			GenerateNextRoom(GetActorLocation() - FVector(400, 0, 0), true, false, false, false);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(WallClass, GetActorLocation() + FVector(0, 0, 0), FRotator(0, 90, 0));
	}
		
	if (bHasRightDoor)
	{
		GetWorld()->SpawnActor<AActor>(WallDoorClass, GetActorLocation() + FVector(0, 400, 0), FRotator::ZeroRotator);
		if (bCanGenRight && !ProcManager->GetMapValue(XCoord + 1, YCoord))
			GenerateNextRoom(GetActorLocation() + FVector(0, 400, 0), false, false, false, true);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(WallClass, GetActorLocation() + FVector(0, 400, 0), FRotator::ZeroRotator);
	}

	if (bHasLeftDoor)
	{
		GetWorld()->SpawnActor<AActor>(WallDoorClass, GetActorLocation() + FVector(0,0, 0), FRotator::ZeroRotator);
		if (bCanGenLeft && !ProcManager->GetMapValue(XCoord - 1, YCoord))
			GenerateNextRoom(GetActorLocation() - FVector(0, 400, 0), false, false, true, false);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(WallClass, GetActorLocation() + FVector(0, 0, 0), FRotator::ZeroRotator);
	}
}

void AProcRoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProcRoom, bHasUpDoor)
	DOREPLIFETIME(AProcRoom, bHasDownDoor)
	DOREPLIFETIME(AProcRoom, bHasRightDoor)
	DOREPLIFETIME(AProcRoom, bHasLeftDoor)

	DOREPLIFETIME(AProcRoom, bCanGenUp)
	DOREPLIFETIME(AProcRoom, bCanGenDown)
	DOREPLIFETIME(AProcRoom, bCanGenRight)
	DOREPLIFETIME(AProcRoom, bCanGenLeft)

	DOREPLIFETIME(AProcRoom, bIsStart)

	DOREPLIFETIME(AProcRoom, RoomNumber)

	DOREPLIFETIME(AProcRoom, XCoord)
	DOREPLIFETIME(AProcRoom, YCoord)

	DOREPLIFETIME(AProcRoom, Node)
}

void AProcRoom::GenerateNextRoom_Implementation(FVector SpawnLocation, bool bUpRequired, bool bDownRequired, bool bRightRequired, bool bLeftRequired)
{
	// Only generate rooms with the ability to generate more rooms if under the MaxRoomNumber as defined in the ProcManager
	if (ProcManager->Rooms.Num() < ProcManager->MaxRoomNumber)
	{
		// Spawn the room
		AProcRoom* NewRoom = GetWorld()->SpawnActor<AProcRoom>(GetClass(), SpawnLocation, FRotator::ZeroRotator);

		// Set the parameters of the NewRoom
		NewRoom->ProcManager = ProcManager;

		UE_LOG(LogTemp, Error, TEXT("WHAT"))
		NewRoom->Node = GetWorld()->SpawnActor<APickupNode>(NodeClass, SpawnLocation + FVector(200, 200, 5), FRotator::ZeroRotator);
		
		if (bUpRequired)
		{
			NewRoom->bHasUpDoor = true;
			NewRoom->bCanGenUp = false;
			NewRoom->XCoord = XCoord;
			NewRoom->YCoord = YCoord - 1;
		}
		if (bDownRequired)
		{
			NewRoom->bHasDownDoor = true;
			NewRoom->bCanGenDown = false;
			NewRoom->XCoord = XCoord;
			NewRoom->YCoord = YCoord + 1;
		}
		if (bRightRequired)
		{
			NewRoom->bHasRightDoor = true;
			NewRoom->bCanGenRight = false;
			NewRoom->XCoord = XCoord - 1;
			NewRoom->YCoord = YCoord;
		}
		if (bLeftRequired)
		{
			NewRoom->bHasLeftDoor = true;
			NewRoom->bCanGenLeft = false;
			NewRoom->XCoord = XCoord + 1;
			NewRoom->YCoord = YCoord;
		}
		ProcManager->Rooms.Add(NewRoom);
		NewRoom->RoomNumber = ProcManager->Rooms.Num();

		// If under the MaxRoomNumber threshold, then randomly set the next room to generate additional rooms
		if (RoomNumber < ProcManager->MaxRoomNumber - 1)
		{
			// Check if there is already a room:
			// Up
			if (!ProcManager->GetMapValue(NewRoom->XCoord, NewRoom->YCoord + 1))
				NewRoom->bHasUpDoor = FMath::RandBool();

			// Down
			if (!ProcManager->GetMapValue(NewRoom->XCoord, NewRoom->YCoord - 1))
				NewRoom->bHasDownDoor = FMath::RandBool();

			// Right
			if (!ProcManager->GetMapValue(NewRoom->XCoord + 1, NewRoom->YCoord))
				NewRoom->bHasRightDoor = FMath::RandBool();

			// Left
			if (!ProcManager->GetMapValue(NewRoom->XCoord - 1, NewRoom->YCoord))
				NewRoom->bHasLeftDoor = FMath::RandBool();
		}

		// Small room numbers have an 90% chance to spawn additional walls
		if (RoomNumber < 3)
		{
			if (!ProcManager->GetMapValue(NewRoom->XCoord, NewRoom->YCoord + 1))
				NewRoom->bHasUpDoor = FMath::RandRange(0.0f, 1.0f) > 0.1f;

			if (!ProcManager->GetMapValue(NewRoom->XCoord, NewRoom->YCoord - 1))
				NewRoom->bHasDownDoor = FMath::RandRange(0.0f, 1.0f) > 0.1f;

			if (!ProcManager->GetMapValue(NewRoom->XCoord + 1, NewRoom->YCoord))
				NewRoom->bHasRightDoor = FMath::RandRange(0.0f, 1.0f) > 0.1f;

			if (!ProcManager->GetMapValue(NewRoom->XCoord - 1, NewRoom->YCoord))
				NewRoom->bHasLeftDoor = FMath::RandRange(0.0f, 1.0f) > 0.1f;
		}

		// Assign the grid value
		ProcManager->SetMapValue(NewRoom->XCoord, NewRoom->YCoord);

		// Call the GenerateWalls function of the NewRoom
		NewRoom->GenerateWalls();
	}
	// If the maximum amount of rooms have been reached, generate the ending rooms, that cannot generate additional rooms
	else
	{
		// Spawn the room
		AProcRoom* NewRoom = GetWorld()->SpawnActor<AProcRoom>(GetClass(), SpawnLocation, FRotator::ZeroRotator);

		// Set the parameters
		if (bUpRequired)
		{
			NewRoom->bHasUpDoor = true;
			NewRoom->XCoord = XCoord;
			NewRoom->YCoord = YCoord - 1;
		}
		if (bDownRequired)
		{
			NewRoom->bHasDownDoor = true;
			NewRoom->XCoord = XCoord;
			NewRoom->YCoord = YCoord + 1;
		}
		if (bRightRequired)
		{
			NewRoom->bHasRightDoor = true;
			NewRoom->XCoord = XCoord - 1;
			NewRoom->YCoord = YCoord;
		}
		if (bLeftRequired)
		{
			NewRoom->bHasLeftDoor = true;
			NewRoom->XCoord = XCoord + 1;
			NewRoom->YCoord = YCoord;
		}

		// Assign the grid value
		ProcManager->SetMapValue(NewRoom->XCoord, NewRoom->YCoord);

		NewRoom->bCanGenUp = false;
		NewRoom->bCanGenDown = false;
		NewRoom->bCanGenRight = false;
		NewRoom->bCanGenLeft = false;

		NewRoom->Node = GetWorld()->SpawnActor<APickupNode>(NodeClass, SpawnLocation + FVector(200, 200, 5), FRotator::ZeroRotator);

		NewRoom->ProcManager = ProcManager;
		ProcManager->Rooms.Add(NewRoom);
		NewRoom->RoomNumber = ProcManager->Rooms.Num();

		NewRoom->GenerateWalls();
	}
}
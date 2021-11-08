// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProcRoom.generated.h"

class AProcManager;
UCLASS()
class MYPROJECT_API AProcRoom : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AProcRoom();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Room)
	UStaticMeshComponent* Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Room)
	TSubclassOf<AActor> WallClass;

	UPROPERTY(EditAnywhere, Category=Room)
	TSubclassOf<AActor> StartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Room)
	TSubclassOf<AActor> WallDoorClass;

	UPROPERTY(VisibleAnywhere, Category=Room)
	bool bHasUpDoor;

	UPROPERTY(VisibleAnywhere, Category=Room)
	bool bHasDownDoor;
	
	UPROPERTY(VisibleAnywhere, Category=Room)
	bool bHasRightDoor;

	UPROPERTY(VisibleAnywhere, Category=Room)
	bool bHasLeftDoor;

	bool bCanGenUp;
	bool bCanGenDown;
	bool bCanGenRight;
	bool bCanGenLeft;

	bool bIsStart;
	
	UPROPERTY(VisibleAnywhere, Category=Room)
	int RoomNumber;

	AProcManager* ProcManager;

	UPROPERTY(VisibleAnywhere, Category=Room)
	int XCoord;

	UPROPERTY(VisibleAnywhere, Category=Room)
	int YCoord;
	
	void GenerateWalls();
	void GenerateNextRoom(FVector SpawnLocation, bool bUpRequired, bool bDownRequired, bool bRightRequired, bool bLeftRequired);
};
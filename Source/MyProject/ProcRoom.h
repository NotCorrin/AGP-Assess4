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

	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	bool bHasUpDoor;

	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	bool bHasDownDoor;
	
	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	bool bHasRightDoor;

	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	bool bHasLeftDoor;

	UPROPERTY(Replicated)
	bool bCanGenUp;

	UPROPERTY(Replicated)
	bool bCanGenDown;

	UPROPERTY(Replicated)
	bool bCanGenRight;
	
	UPROPERTY(Replicated)
	bool bCanGenLeft;

	UPROPERTY(Replicated)
	bool bIsStart;
	
	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	int RoomNumber;

	AProcManager* ProcManager;

	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	int XCoord;

	UPROPERTY(VisibleAnywhere, Category=Room, Replicated)
	int YCoord;
	
	void GenerateWalls();

	UFUNCTION(Server, Reliable)
	void GenerateNextRoom(FVector SpawnLocation, bool bUpRequired, bool bDownRequired, bool bRightRequired, bool bLeftRequired);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
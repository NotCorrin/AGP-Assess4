// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProcRoom.h"
#include "GameFramework/Actor.h"
#include "ProcManager.generated.h"

UCLASS()
class MYPROJECT_API AProcManager : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AProcManager();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:
	UPROPERTY(EditAnywhere, Category="Procedural Rooms")
	TSubclassOf<AProcRoom> Room;

	UPROPERTY(EditAnywhere, Category="Procedural Rooms")
	TSubclassOf<ANavigationNode> NodeClass;

	int MaxRoomNumber;
	
	TArray<AProcRoom*> Rooms;

	TArray<bool> LevelMap;
	bool GetMapValue(int XIndex, int YIndex);
	void SetMapValue(int XIndex, int YIndex);

	UFUNCTION(Server, Reliable)
	void GenerateStartingRoom();
};

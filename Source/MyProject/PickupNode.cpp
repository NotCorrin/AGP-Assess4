// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupNode.h"

// Sets default values
APickupNode::APickupNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Same code as Navigation Node
	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	RootComponent = LocationComponent;
}

// Called when the game starts or when spawned
void APickupNode::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


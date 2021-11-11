// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "PlayerCharacter.h"
#include "Engine/GameEngine.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pickup Scene Component"));
	RootComponent = PickupSceneComponent;

	PickupBoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Bounding Box"));
	PickupBoundingBox->AttachTo(RootComponent);
	PickupBoundingBox->SetGenerateOverlapEvents(true);
	PickupBoundingBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnEnterPickup);
	PickupBoundingBox->SetWorldScale3D(FVector(1.0f, 2.0f, 1.0f));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnEnterPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnPickup(OtherActor);
}

void APickup::OnPickup(AActor* ActorThatPickedUp)
{

}

void APickup::OnGenerate()
{

}

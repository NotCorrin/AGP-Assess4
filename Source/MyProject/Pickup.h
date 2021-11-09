// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Pickup.generated.h"

UCLASS()
class MYPROJECT_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USceneComponent* PickupSceneComponent;

	UBoxComponent* PickupBoundingBox;

	APlayerCharacter* PlayerCharacter;

	AEnemyCharacter* EnemyCharacter;

	UPROPERTY(BlueprintReadWrite)
		bool bSetMaterial;

	bool bMovementPickedUp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 WhichPowerup;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 WhichMovementPowerup;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 WhichHealthPowerup;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 WhichArmorPowerup;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 WhichWeaponShotType;


	class AAIManager* AIManager;
	class APickupManager* PickupManager;


	UFUNCTION()

		virtual void OnEnterPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnPickup(AActor* ActorThatPickedUp);

	virtual void OnGenerate();

	virtual void OnSprintPickup(AActor* ActorThatPickedUp);

	virtual void OnJumpPickup(AActor* ActorThatPickedUp);

	virtual void OnInstantHealthPickup(AActor* ActorThatPickedUp);

	virtual void OnHOTPickup(AActor* ActorThatPickedUp);

	virtual void OnLightArmorPickup(AActor* ActorThatPickedUp);

	virtual void OnHeavyArmorPickup(AActor* ActorThatPickedUp);

	virtual void OnTripleShotPickup(AActor* ActorThatPickedUp);

	virtual void OnAutomaticPickup(AActor* ActorThatPickedUp);

	virtual void OnExplosivePickup(AActor* ActorThatPickedUp);

	virtual void OnAmmoPickup(AActor* ActorThatPickedUp);


};

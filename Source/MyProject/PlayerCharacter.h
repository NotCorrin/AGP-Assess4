// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "HealthComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MYPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	float LookSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintMultiplier;

	bool bIsSprinting;
	bool bSprintPickedup;
	bool bJumpPickedUp;
	bool bHOTPickedUp;

	UPROPERTY(BlueprintReadOnly)
	bool bDamagePickedUp;

	UPROPERTY(BlueprintReadWrite)
	int32 RoundsRemaining;

	UHealthComponent* HealthComponent;

	int32 SprintTimer;
	int32 JumpTimer;
	int32 HOTTimer;
	int32 HOTEndTimer;
	int32 DamageTimer;

	float PickupSprintSpeed;
	float PickupJumpHeight;

	UPROPERTY(BlueprintReadOnly)
	float IncreasedDamage;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void Strafe(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void SprintStart();
	void SprintEnd();

	UFUNCTION(Server, Reliable)
	void SetSpawnPoint();

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintReload();

	void Reload();

	void RemoveSprintPickup();

	void RemoveJumpPickup();

	void InstantHealthPickup();

	void HOTPickup();

	UFUNCTION(Server, Reliable)
	void ServerSprintStart();

	UFUNCTION(Server, Reliable)
	void ServerSprintEnd();

	UFUNCTION(Server, Reliable)
	void AlreadySprinting();

	UFUNCTION(Server, Reliable)
	void SprintSpeed();

	UFUNCTION(Server, Reliable)
	void IncreaseJump();

	UFUNCTION(Server, Reliable)
	void DecreaseJump();

	void OnDeath();

	UFUNCTION(Client, Reliable)
	void SetGameOver();

	void RemoveIncreasedDamagePickup();

protected:

	float SprintMovemementSpeed;
	float NormalMovementSpeed;

	float NormalJumpHeight;

private:

	UCameraComponent* Camera;
	class UFirstPersonAnimInstance* AnimInstance;
};

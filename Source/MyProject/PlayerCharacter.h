// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "HealthComponent.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerWeaponShotType : uint8
{
	SINGLE_SHOT,
	TRIPLE_SHOT,
	AUTOMATIC,
	EXPLOSIVE
};

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
	bool bWeaponShotTypePickedUp;

	UPROPERTY(BlueprintReadWrite)
		bool bAmmoPickedUp;

	UPROPERTY(BlueprintReadOnly)
		int32 AmmoSize;

	UPROPERTY(BlueprintReadWrite)
		int32 RoundsRemaining;

	UHealthComponent* HealthComponent;

	int32 SprintTimer;
	int32 JumpTimer;
	int32 HOTTimer;
	int32 HOTEndTimer;
	int32 ShotTypeTimer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EPlayerWeaponShotType WeaponShotType;


	float PickupSprintSpeed;
	float PickupJumpHeight;

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

	UFUNCTION(BlueprintImplementableEvent)
		void BlueprintReload();

	void Reload();

	void RemoveSprintPickup();

	void RemoveJumpPickup();

	void InstantHealthPickup();

	void HOTPickup();

	void RemoveWeaponShotType();



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

protected:

	float SprintMovemementSpeed;
	float NormalMovementSpeed;

	float NormalJumpHeight;

private:

	UCameraComponent* Camera;
	class UFirstPersonAnimInstance* AnimInstance;
};

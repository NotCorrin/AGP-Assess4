// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "HealthComponent.h"
#include "Components/InputComponent.h"
#include "FirstPersonAnimInstance.h"
#include "TimerManager.h"
#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Set default member variable values
	LookSensitivity = 1.0f;
	SprintMultiplier = 1.5f;

	bIsSprinting = false;

	bSprintPickedup = false;
	bJumpPickedUp = false;
	bHOTPickedUp = false;
	bWeaponShotTypePickedUp = false;
	bAmmoPickedUp = false;

	SprintTimer = 0;
	JumpTimer = 0;
	HOTTimer = 0;
	HOTEndTimer = 0;
	ShotTypeTimer = 0;

	AmmoSize = 5;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Initialise the camera variable
	Camera = FindComponentByClass<UCameraComponent>();

	// Get the skeletal mesh and then get the anim instance from it cast to the first person anim instance.
	USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(GetDefaultSubobjectByName(TEXT("Arms")));
	if (SkeletalMesh) // Make sure the skeletal mesh was found
	{
		AnimInstance = Cast<UFirstPersonAnimInstance>(SkeletalMesh->GetAnimInstance());
	}

	HealthComponent = FindComponentByClass<UHealthComponent>();		//initialises the health component

	WeaponShotType = EPlayerWeaponShotType::SINGLE_SHOT;	//sets the player's gun shot type to SINGLE_SHOT
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprintPickedup && SprintTimer == 0)		//checks if the player has picked up a sprint power up and if the SprintTimer is set to 0
	{
		SprintTimer = 1;	//sets timer to 1 so the RemoveSprintPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveSprintPickup, 5.f, false);	//calls the RemoveSprintPickup function after 5 seconds
	}
	else if (bJumpPickedUp && JumpTimer == 0)	//checks if the player has picked up a jump power up and if the JumpTimer is set to 0
	{
		JumpTimer = 1;	//sets timer to 1 so the RemoveJumpPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveJumpPickup, 5.f, false);		//calls the RemoveJumpPickup function after 5 seconds
	}
	else if (bHOTPickedUp && HOTTimer == 0)		//checks if the player has picked up a HOT power up and if the HOTTimer is set to 0
	{
		HOTTimer = 1;	//sets timer to 1 so the HOTPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::HOTPickup, 1.f, true);		//calls and loops the HOTPickup function every 1 seconds

		HOTEndTimer = 0;	//after all health points have been added resets timer fo next pick up
	}
	else if (bWeaponShotTypePickedUp && ShotTypeTimer == 0)		//checks if the weapon shot type power up has been picked up and if the ShotTypeTimer is set to 0
	{
		ShotTypeTimer = 1;	//sets timer to 1 so the RemoveWeaponShotTypePickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveWeaponShotType, 10.f, true);		//calls the RemoveWeaponShotTypePickup function after 10 seconds
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SprintEnd);

	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);

}

void APlayerCharacter::MoveForward(float Value)
{
	FRotator ForwardRotation = GetControlRotation();
	ForwardRotation.Roll = 0.0f;
	ForwardRotation.Pitch = 0.0f;
	AddMovementInput(ForwardRotation.Vector(), Value);
}

void APlayerCharacter::Strafe(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::LookUp(float Value)
{
	FRotator LookUpRotation = FRotator::ZeroRotator;
	LookUpRotation.Pitch = Value * LookSensitivity;
	if (Camera->RelativeRotation.Pitch + LookUpRotation.Pitch < 90.0f
		&& Camera->RelativeRotation.Pitch + LookUpRotation.Pitch > -90.0f)
	{
		Camera->AddRelativeRotation(LookUpRotation);
		Camera->RelativeRotation.Yaw = 0.0f;
		Camera->RelativeRotation.Roll = 0.0f;
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void APlayerCharacter::SprintStart()
{
	bIsSprinting = true;

	if (bSprintPickedup)	//checks if the player has picked up a sprint power up
	{
		GetCharacterMovement()->MaxWalkSpeed *= (SprintMultiplier + 5.0f);		//increases sprint speed
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
	}

	if (AnimInstance)
	{
		AnimInstance->bIsSprinting = true;
	}
}

void APlayerCharacter::SprintEnd()
{
	bIsSprinting = false;

	if (bSprintPickedup)	//checks if the player has picked up a sprint power up
	{
		GetCharacterMovement()->MaxWalkSpeed /= (SprintMultiplier + 5.0f);	//decreases sprint speed back to original Max Walk Speed if the plyaer has stopped sprinting
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
	}

	if (AnimInstance)
	{
		AnimInstance->bIsSprinting = false;
	}
}

void APlayerCharacter::Reload()
{
	BlueprintReload();
}

void APlayerCharacter::RemoveSprintPickup()
{
	bSprintPickedup = false;	//resets for next pick up

	if (bIsSprinting)	//checks if the player is still sprinting
	{
		GetCharacterMovement()->MaxWalkSpeed /= (SprintMultiplier + 5.0f);	//decreases sprint speed back to original Max Walk Speed if the player is still sprinting
	}

	SprintTimer = 0; //resets for next pick up
}

void APlayerCharacter::RemoveJumpPickup()
{
	bJumpPickedUp = false;	//resets for next pick up

	GetCharacterMovement()->JumpZVelocity -= 300.0f;	//decreases Jump Z Velocity back to original value

	JumpTimer = 0;	//resets for next pick up
}

void APlayerCharacter::HOTPickup()
{
	if (HealthComponent->CurrentHealth < HealthComponent->MaxHealth)	//checks if the player's Current Health is less than its Max Health
	{
		HOTEndTimer += 1;

		if (HOTEndTimer <= 6.0f)	//checks if it has been less than 6 seconds
		{
			HealthComponent->CurrentHealth += 5.0f;		//increases player's Current Health

			if (HealthComponent->CurrentHealth > HealthComponent->MaxHealth)	//checks if after the increase, the player's Currrent Health is greater than its Max Health
			{
				HealthComponent->CurrentHealth = HealthComponent->MaxHealth;	//Makes the player's Current Health equal to its Max Health
			}

			GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), HealthComponent->CurrentHealth));	//prints its Current Health to the screen
		}
		else
		{
			bHOTPickedUp = false;	//resets for next pick up

			HOTTimer = 0;	//resets for next pick up
		}
	}
}

void APlayerCharacter::RemoveWeaponShotType()
{
	bWeaponShotTypePickedUp = false;	//resets for next pick up

	WeaponShotType = EPlayerWeaponShotType::SINGLE_SHOT;	//returns shot type back to original value

	ShotTypeTimer = 0;	//resets for next pick up
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EngineUtils.h"
#include "HealthComponent.h"
#include "Components/InputComponent.h"
#include "FirstPersonAnimInstance.h"
#include "TimerManager.h"
#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerGameMode.h"
#include "PlayerHUD.h"
#include "ProcManager.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Set default member variable values
	LookSensitivity = 1.0f;
	SprintMultiplier = 1.5f;

	bIsSprinting = false;

	bSprintPickedup = false;
	bJumpPickedUp = false;
	bHOTPickedUp = false;
	bDamagePickedUp = false;

	IncreasedDamage = 100.0f;

	SprintTimer = 0;
	JumpTimer = 0;
	HOTTimer = 0;
	HOTEndTimer = 0;

	SprintMovemementSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintMultiplier;
	NormalMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	PickupSprintSpeed = GetCharacterMovement()->MaxWalkSpeed * (SprintMultiplier + 5.0f);

	PickupJumpHeight = GetCharacterMovement()->JumpZVelocity + 300.0f;
	NormalJumpHeight = GetCharacterMovement()->JumpZVelocity;

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

	if (HealthComponent != NULL)
	{
		HealthComponent->SetIsReplicated(true);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::SetSpawnPoint, 0.5f, false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprintPickedup && SprintTimer == 0)		//checks if the player has picked up a sprint pickup and if the SprintTimer is set to 0
	{
		SprintTimer = 1;	//sets timer to 1 so the RemoveSprintPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveSprintPickup, 5.f, false);	//calls the RemoveSprintPickup function after 5 seconds
	}
	else if (bJumpPickedUp && JumpTimer == 0)	//checks if the player has picked up a jump pickup and if the JumpTimer is set to 0
	{
		JumpTimer = 1;	//sets timer to 1 so the RemoveJumpPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveJumpPickup, 5.f, false);		//calls the RemoveJumpPickup function after 5 seconds
	}
	else if (bHOTPickedUp && HOTTimer == 0)		//checks if the player has picked up a HOT pickup and if the HOTTimer is set to 0
	{
		HOTTimer = 1;	//sets timer to 1 so the HOTPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::HOTPickup, 1.f, true);		//calls and loops the HOTPickup function every 1 seconds

		HOTEndTimer = 0;	//after all health points have been added resets timer fo next pickup
	}
	else if (bDamagePickedUp && DamageTimer == 0)	//checks if the player has picked up an increased damage pickup and if the DamageTimer is set to 0
	{
		DamageTimer = 1;	//sets timer to 1 so the RemoveIncreasedDamagePickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::RemoveIncreasedDamagePickup, 5.f, false);		//calls the RemoveIncreasedDamagePickup function after 5 seconds
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent != NULL)
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::Strafe);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);

		PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SprintStart);
		PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SprintEnd);
	}
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

	if (Camera != NULL)
	{
		if (Camera->RelativeRotation.Pitch + LookUpRotation.Pitch < 90.0f
			&& Camera->RelativeRotation.Pitch + LookUpRotation.Pitch > -90.0f)
		{
			Camera->AddRelativeRotation(LookUpRotation);
			Camera->RelativeRotation.Yaw = 0.0f;
			Camera->RelativeRotation.Roll = 0.0f;
		}
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void APlayerCharacter::SprintStart()
{
	bIsSprinting = true;

	if (bSprintPickedup)	//checks if the player has picked up a sprint pickup
	{
		GetCharacterMovement()->MaxWalkSpeed = PickupSprintSpeed;	//changes max walk speed to increased sprint speed
		ServerSprintStart();
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMovemementSpeed;	//changes max walk speed to original sprint speed
		ServerSprintStart();
	}

	if (AnimInstance)
	{
		AnimInstance->bIsSprinting = true;
	}

}

void APlayerCharacter::SprintEnd()
{
	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;	//changes max walk speed to normal walk speed
	ServerSprintEnd();

	if (AnimInstance)
	{
		AnimInstance->bIsSprinting = false;
	}

}

void APlayerCharacter::SetSpawnPoint_Implementation()
{
	TArray<AProcRoom*> AllRooms;
	for (TActorIterator<AProcRoom> It(GetWorld()); It; ++It)
	{
		AllRooms.Add(*It);    //Adds all rooms to an array
	}
	// Select a random room from AllRooms, then teleport to it
	AProcRoom* RandomRoom = AllRooms[FMath::RandRange(0, AllRooms.Num() - 1)];
	TeleportTo(RandomRoom->GetActorLocation() + FVector(80, 200, 5), FRotator::ZeroRotator);
}

void APlayerCharacter::ServerSprintStart_Implementation()
{
	if (bSprintPickedup)	//checks if the player character has picked up a sprint pickup
	{
		GetCharacterMovement()->MaxWalkSpeed = PickupSprintSpeed;	//changes max walk speed to increased sprint speed
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMovemementSpeed;	//changes max walk speed to original sprint speed
	}
}

void APlayerCharacter::ServerSprintEnd_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;		//changes max walk speed to normal walk speed
}

void APlayerCharacter::AlreadySprinting_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = PickupSprintSpeed;	//changes max walk speed to increased sprint speed
}

void APlayerCharacter::SprintSpeed_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintMovemementSpeed;	//changes max walk speed to original sprint speed
}

void APlayerCharacter::Reload()
{
	BlueprintReload();
}

void APlayerCharacter::RemoveSprintPickup()
{

	bSprintPickedup = false;	//resets for next pickup

	if (bIsSprinting)	//checks if the player is still sprinting
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMovemementSpeed;	//changes max walk speed back to original sprint speed if player is still sprinting
		SprintSpeed();
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;		//changes max walk speed back to normal walk speed
		ServerSprintEnd();
	}

	SprintTimer = 0; //resets for next pickup

}

void APlayerCharacter::IncreaseJump_Implementation()
{
	GetCharacterMovement()->JumpZVelocity = PickupJumpHeight;	//changes the jump height so the player can jump higher than normal
}

void APlayerCharacter::DecreaseJump_Implementation()
{
	GetCharacterMovement()->JumpZVelocity = NormalJumpHeight;	//changes the jump height back to its original value
}

void APlayerCharacter::RemoveJumpPickup()
{
	bJumpPickedUp = false;	//resets for next pickup

	GetCharacterMovement()->JumpZVelocity = NormalJumpHeight;	//changes the jump height back to its original value
	DecreaseJump();

	JumpTimer = 0;	//resets for next pickup
}

void APlayerCharacter::InstantHealthPickup()
{
	if (GetLocalRole() == ROLE_Authority)	//checks if the player character is on the server
	{
		if (HealthComponent->CurrentHealth < HealthComponent->MaxHealth)	//checks if the player's current health is less than its max health
		{
			HealthComponent->CurrentHealth += 15.0f;	//increases the player's current health

			if (HealthComponent->CurrentHealth > HealthComponent->MaxHealth)		//checks if the player's current health is greater than its max health after the 15 was added
			{
				HealthComponent->CurrentHealth = HealthComponent->MaxHealth;		//sets the players current health to its max health
			}
		}
	}
}

void APlayerCharacter::HOTPickup()
{
	if (HealthComponent && GetLocalRole() == ROLE_Authority)	//checks if the player has a health component and if it is on the server
	{
		if (HealthComponent->CurrentHealth < HealthComponent->MaxHealth)	//checks if the player's current health is less than its max health
		{
			HOTEndTimer += 1;

			if (HOTEndTimer <= 6.0f)	//checks if it has been less than 6 seconds
			{
				HealthComponent->CurrentHealth += 5.0f;		//increases player's current health

				if (HealthComponent->CurrentHealth > HealthComponent->MaxHealth)	//checks if after the increase, the player's currrent health is greater than its max health
				{
					HealthComponent->CurrentHealth = HealthComponent->MaxHealth;	//sets the player's current health equal to its max health
				}
			}
			else
			{
				bHOTPickedUp = false;	//resets for next pickup
				HOTTimer = 0;	//resets for next pickup
			}
		}
	}
}

void APlayerCharacter::OnDeath()
{
	if (GetLocalRole() == ROLE_Authority)	//checks if the player is on the server
	{
		AMultiplayerGameMode* MultiplayerGameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
		if (MultiplayerGameMode)
		{
			MultiplayerGameMode->GameOver(GetController());
		}
	}
}

void APlayerCharacter::SetGameOver_Implementation()
{
	if (GetLocalRole() == ROLE_AutonomousProxy || this->IsLocallyControlled())	//checks if the player is an autonomous proxy on the client or it is being locally controlled
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
			{
				PlayerHUD->SetGameOver();	//calls function to get game over screen to appear on HUD
			}
		}
	}
}

void APlayerCharacter::RemoveIncreasedDamagePickup()
{
	bDamagePickedUp = false;	//resets for next pickup
	DamageTimer = 0;	//resets for next pickup
}
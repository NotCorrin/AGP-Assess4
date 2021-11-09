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

	bSetMaterial = false;
	bMovementPickedUp = false;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	WhichPowerup = FMath::RandRange(1, 100);				//decides which powerup will spawn
	WhichMovementPowerup = FMath::RandRange(1, 100);
	WhichHealthPowerup = FMath::RandRange(1, 100);			//if it is a health powerup, decides which health powerup will spawn, instant health or health over time
	WhichArmorPowerup = FMath::RandRange(1, 100);			//if it is a armor powerup, decides which armor powerup will spawn, light or heavy armor
	WhichWeaponShotType = FMath::RandRange(1, 100);			//if it is a weapon shot type powerup, decides which shot type it will spawn, triple shot, automatic or explosive

	bSetMaterial = true;	//allows for the material of the pickup to be set to the material of whichever powerup was chosen

}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnEnterPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	/*if (WhichPowerup <= 15)
	{
		if (WhichMovementPowerup <= 50)
		{
			OnSprintPickup(OtherActor);		//increases the player character's sprint speed by 5.0f for 5 seconds
		}
		else
		{
			OnJumpPickup(OtherActor);		//increases the player character's jump height by 300.0f for 5 seconds
		}
	}
	else if (WhichPowerup > 15 && WhichPowerup <= 40)
	{
		if (WhichHealthPowerup <= 70)
		{
			OnInstantHealthPickup(OtherActor);		//increases the actor's current health, player or enemy character, by 15.0f instantly
		}
		else
		{
			OnHOTPickup(OtherActor);	//increases the actor's current health, player or enemy character, by 30.0f instantly over 12 seconds
		}
	}
	else if (WhichPowerup > 40 && WhichPowerup <= 60)
	{
		if (WhichArmorPowerup <= 60)
		{
			OnLightArmorPickup(OtherActor);		//allows the actor, player or enemy character, to get shot 3 times before taking damage
		}
		else
		{
			OnHeavyArmorPickup(OtherActor);		//allows the actor, player or enemy character, to get shot 8 times before taking damage
		}
	}
	else if (WhichPowerup > 60 && WhichPowerup <= 70)
	{
		if (WhichWeaponShotType <= 45)
		{
			OnTripleShotPickup(OtherActor);		//instead of firing one bullet every left mouse button click, it fires three bullets every left mouse button click for 10 seconds
		}
		else if (WhichWeaponShotType > 45 && WhichWeaponShotType <= 80)
		{
			OnAutomaticPickup(OtherActor);		//if the player holds down the left mouse button the gun will keep firing until either the player runs out of bullets or the left mouse button is released for 10 seconds
		}
		else
		{
			OnExplosivePickup(OtherActor);		//increases the damage of the bullets by 5.0f for 10 seconds
		}
	}
	else
	{
		OnAmmoPickup(OtherActor);	//increases the amount of ammo the player can hold by 5
	}*/

	OnPickup(OtherActor);
}

void APickup::OnPickup(AActor* ActorThatPickedUp)
{

}

void APickup::OnGenerate()
{

}

void APickup::OnSprintPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())			//checks if the actor that entered the pickup was a player character
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);		//allows for the pickup to access the player character script

		if (PlayerCharacter->bIsSprinting && !PlayerCharacter->bSprintPickedup)		//checks if the player is already sprinting and if the player hasn't already picked up a sprint powerup
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= (PlayerCharacter->SprintMultiplier + 5.0f);	//increases the player's sprint speed by 5.0f

			PlayerCharacter->bSprintPickedup = true;	//indicates that the player has picked up a sprint pickup to prevent them from picking up multiple until the timer has finished

			Destroy();		//removes the pickup from the level
		}
		else if (!PlayerCharacter->bIsSprinting && !PlayerCharacter->bSprintPickedup)		//checks if the player isn't sprinting and if the player hasn't already picked up a sprint powerup
		{
			PlayerCharacter->bSprintPickedup = true;

			Destroy();
		}
	}
}

void APickup::OnJumpPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bJumpPickedUp)		//checks if the player hasn't already picked up a jump powerup
		{
			PlayerCharacter->GetCharacterMovement()->JumpZVelocity += 300.0f;	//increases the player's jump height by 300.0f

			PlayerCharacter->bJumpPickedUp = true;		//indicates that the player has picked up a jump powerup to prevent them from picking up multiple until the timer has finished

			Destroy();
		}
	}
}

void APickup::OnInstantHealthPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent != NULL && PlayerCharacter->HealthComponent->CurrentHealth < PlayerCharacter->HealthComponent->MaxHealth)	//checks if the player has a health component and if the player's current health is less than its max health
		{
			PlayerCharacter->HealthComponent->CurrentHealth += 15.0f;

			if (PlayerCharacter->HealthComponent->CurrentHealth > PlayerCharacter->HealthComponent->MaxHealth)		//checks if the player's current health is greater than its max health after the 15.0f was added
			{
				PlayerCharacter->HealthComponent->CurrentHealth = PlayerCharacter->HealthComponent->MaxHealth;		//sets the players current health to its max health
			}

			GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), PlayerCharacter->HealthComponent->CurrentHealth));		//prints the player's current health to the screen

			Destroy();
		}
	}
	else if (ActorThatPickedUp->IsA<AEnemyCharacter>())
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorThatPickedUp);

		if (EnemyCharacter->HealthComponent != NULL && EnemyCharacter->HealthComponent->CurrentHealth < EnemyCharacter->HealthComponent->MaxHealth)		//checks if the enemy has a health component and if the enemy's current health is less than its max health
		{
			EnemyCharacter->HealthComponent->CurrentHealth += 15.0f;

			if (EnemyCharacter->HealthComponent->CurrentHealth > EnemyCharacter->HealthComponent->MaxHealth)		//checks if the enemy's current health is greater than its max health after the 15.0f was added
			{
				EnemyCharacter->HealthComponent->CurrentHealth = EnemyCharacter->HealthComponent->MaxHealth;		//sets the enemy's current health to its max health
			}

			GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Red, FString::Printf(TEXT("Current Enemy Health: %f"), EnemyCharacter->HealthComponent->CurrentHealth));	//prints the enemy's current health to the screen

			Destroy();
		}
	}
}

void APickup::OnHOTPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent != NULL && PlayerCharacter->HealthComponent->CurrentHealth < PlayerCharacter->HealthComponent->MaxHealth && !PlayerCharacter->bHOTPickedUp)	//checks if the player has a health component and if the player's current health is less than the player's max health and if the player hasn't already picked up a HOT(Health Over Time) powerup
		{
			PlayerCharacter->bHOTPickedUp = true;		//indicates that the player has picked up a HOT powerup to prevent them from picking up multiple until the timer has finished

			Destroy();
		}
	}
	else if (ActorThatPickedUp->IsA<AEnemyCharacter>())
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorThatPickedUp);

		if (EnemyCharacter->HealthComponent != NULL && EnemyCharacter->HealthComponent->CurrentHealth < EnemyCharacter->HealthComponent->MaxHealth && !EnemyCharacter->bHOTPickedUp)	//checks if the enemy has a health component and if the enemy's current health is less than its max health and if the enemy hasn't already picked up a HOT(Health Over Time) powerup
		{
			EnemyCharacter->bHOTPickedUp = true;	//indicates that the enemy has picked up a HOT powerup to prevent them from pickup up multiple until the timer has finished

			Destroy();
		}
	}
}

void APickup::OnLightArmorPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent->TakeDamage == 0)	//checks if the player hasn't already picked up an armor powerup to prevent them from picking up multiple until their TakeDamage is equal to 0 again
		{
			PlayerCharacter->HealthComponent->TakeDamage = 3;	//sets the player's Take Damage variable to 3

			Destroy();
		}
	}
	else if (ActorThatPickedUp->IsA<AEnemyCharacter>())
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorThatPickedUp);

		if (EnemyCharacter->HealthComponent->TakeDamage == 0)	//checks if the enemy hasn't already picked up an armor powerup to prevent them from picking up multiple until their TakeDamage is equal to 0 again
		{
			EnemyCharacter->HealthComponent->TakeDamage = 3;	//sets the enemy's Take Damage variable to 3

			Destroy();
		}
	}
}

void APickup::OnHeavyArmorPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent->TakeDamage == 0)
		{
			PlayerCharacter->HealthComponent->TakeDamage = 8;	//sets the player's Take Damage variable to 8

			Destroy();
		}
	}
	else if (ActorThatPickedUp->IsA<AEnemyCharacter>())
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorThatPickedUp);

		if (EnemyCharacter->HealthComponent->TakeDamage == 0)
		{
			EnemyCharacter->HealthComponent->TakeDamage = 8;	//sets the enemy's Take Damage variable to 8

			Destroy();
		}
	}
}

void APickup::OnTripleShotPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bWeaponShotTypePickedUp)		//checks if the player hasn't already picked up a weapon shot type powerup to prevent them from picking up multiple until the timer has finished
		{
			PlayerCharacter->WeaponShotType = EPlayerWeaponShotType::TRIPLE_SHOT;	//changes the player's gun weapon shot type from SINGLE_SHOT to TRIPLE_SHOT

			PlayerCharacter->bWeaponShotTypePickedUp = true;	//indicates that the player has picked up a weapon shot type powerup

			Destroy();
		}
	}
}

void APickup::OnAutomaticPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bWeaponShotTypePickedUp)
		{
			PlayerCharacter->WeaponShotType = EPlayerWeaponShotType::AUTOMATIC;		//changes the player's gun weapon shot type from SINGLE_SHOT to AUTOMATIC

			PlayerCharacter->bWeaponShotTypePickedUp = true;

			Destroy();
		}
	}
}

void APickup::OnExplosivePickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bWeaponShotTypePickedUp)
		{
			PlayerCharacter->WeaponShotType = EPlayerWeaponShotType::EXPLOSIVE;		//changes the player's gun weapon shot type from SINGLE_SHOT to EXPLOSIVE

			PlayerCharacter->bWeaponShotTypePickedUp = true;

			Destroy();
		}
	}
	else if (ActorThatPickedUp->IsA<AEnemyCharacter>())
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorThatPickedUp);

		if (!EnemyCharacter->bWeaponShotTypePickedUp)	//checks if the enemy hasn't already picked up a weapon shot type powerup to prevent them from picking up multiple until the timer has finished
		{
			EnemyCharacter->WeaponShotType = EEnemyWeaponShotType::EXPLOSIVE;	//changes the enemy's gun weapon shot type from SINGLE_SHOT to EXPLOSIVE

			EnemyCharacter->bWeaponShotTypePickedUp = true;		//indicates that the enemy has picked up a weapon shot type powerup

			Destroy();
		}
	}
}

void APickup::OnAmmoPickup(AActor* ActorThatPickedUp)
{
	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		PlayerCharacter->bAmmoPickedUp = true;

		Destroy();

	}
}

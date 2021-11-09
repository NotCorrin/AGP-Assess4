// Fill out your copyright notice in the Description page of Project Settings.


#include "HOTPickup.h"
#include "Engine/GameEngine.h"

void AHOTPickup::OnGenerate()
{
	APickup::OnGenerate();
}

void AHOTPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

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
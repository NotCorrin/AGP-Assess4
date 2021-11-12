// Fill out your copyright notice in the Description page of Project Settings.


#include "HOTPickup.h"

void AHOTPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())		//checks if the actor that picked up the pickup is a player character
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent != NULL && PlayerCharacter->HealthComponent->CurrentHealth < PlayerCharacter->HealthComponent->MaxHealth && !PlayerCharacter->bHOTPickedUp)	//checks if the player has a health component and if the player's current health is less than the player's max health and if the player hasn't already picked up a HOT(Health Over Time) pickup
		{
			PlayerCharacter->bHOTPickedUp = true;		//indicates that the player has picked up a HOT powerup to prevent them from picking up multiple until the timer has finished

			Destroy();	//removes pickup from the world
		}
	}
}
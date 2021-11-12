// Fill out your copyright notice in the Description page of Project Settings.


#include "LightArmorPickup.h"

void ALightArmorPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())		//checks if the actor that picked up the pickup is a player character
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent->TakeDamage == 0)	//checks if the player hasn't already picked up an armor pickup to prevent them from picking up multiple until their TakeDamage is equal to 0 again
		{
			PlayerCharacter->HealthComponent->TakeDamage = 1;	//sets the player's health component Take Damage variable to 1

			Destroy();	//removes pickup from the world
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantHealthPickup.h"

void AInstantHealthPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())		//checks if the actor that picked up the pickup is a player character
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent != NULL && PlayerCharacter->HealthComponent->CurrentHealth < PlayerCharacter->HealthComponent->MaxHealth)	//checks if the player has a health component and if the player's current health is less than its max health
		{
			PlayerCharacter->InstantHealthPickup();		//calls function to increase player character's health instantly

			Destroy();	//Removes pickup from the world
		}
	}
}
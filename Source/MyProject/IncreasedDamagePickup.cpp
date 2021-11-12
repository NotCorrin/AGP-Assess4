// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreasedDamagePickup.h"

void AIncreasedDamagePickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())		//checks if the actor that picked up the powerup is a player character
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bDamagePickedUp)		//checks if the player hasn't already picked up a increased damage powerup
		{
			PlayerCharacter->bDamagePickedUp = true;	//indicates that the player has picked up a IncreasedDamage powerup to prevent them from picking up multiple until the timer has finished

			Destroy();	//removes pickup from the world
		}
	}
}
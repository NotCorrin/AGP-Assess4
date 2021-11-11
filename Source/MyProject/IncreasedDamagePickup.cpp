// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreasedDamagePickup.h"

void AIncreasedDamagePickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bDamagePickedUp)		//checks if the player hasn't already picked up a jump powerup
		{
			PlayerCharacter->bDamagePickedUp = true;

			Destroy();
		}
	}
}
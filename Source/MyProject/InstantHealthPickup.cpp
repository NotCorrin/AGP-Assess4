// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantHealthPickup.h"

void AInstantHealthPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (PlayerCharacter->HealthComponent != NULL && PlayerCharacter->HealthComponent->CurrentHealth < PlayerCharacter->HealthComponent->MaxHealth)	//checks if the player has a health component and if the player's current health is less than its max health
		{
			PlayerCharacter->InstantHealthPickup();

			Destroy();
		}
	}
}
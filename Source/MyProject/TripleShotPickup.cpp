// Fill out your copyright notice in the Description page of Project Settings.


#include "TripleShotPickup.h"

void ATripleShotPickup::OnGenerate()
{
	APickup::OnGenerate();
}

void ATripleShotPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

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

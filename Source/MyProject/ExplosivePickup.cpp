// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosivePickup.h"

void AExplosivePickup::OnGenerate()
{
	APickup::OnGenerate();
}

void AExplosivePickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

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
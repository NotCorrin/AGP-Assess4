// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintPickup.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASprintPickup::OnGenerate()
{
	APickup::OnGenerate();

}

void ASprintPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);		//allows for the pickup to access the player character script

		if (PlayerCharacter->bIsSprinting && !PlayerCharacter->bSprintPickedup)		//checks if the player is already sprinting and if the player hasn't already picked up a sprint pickup
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->PickupSprintSpeed;	//changes player's max walk speed to increased sprint speed
			PlayerCharacter->AlreadySprinting();
			PlayerCharacter->bSprintPickedup = true;	//indicates that the player has picked up a sprint pickup to prevent them from picking up multiple until the timer has finished

			Destroy();		//removes the pickup from the level
		}
		else if (!PlayerCharacter->bIsSprinting && !PlayerCharacter->bSprintPickedup)		//checks if the player isn't sprinting and if the player hasn't already picked up a sprint powerup
		{
			PlayerCharacter->bSprintPickedup = true;	//indicates that the player has picked up a sprint pickup to prevent them from picking up multiple until the timer has finished

			Destroy();	//removes the pickup from the level
		}
	}
}
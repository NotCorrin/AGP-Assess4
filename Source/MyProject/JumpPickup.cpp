// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPickup.h"
#include "GameFramework/CharacterMovementComponent.h"

void AJumpPickup::OnGenerate()
{
	APickup::OnGenerate();
}

void AJumpPickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);

	if (ActorThatPickedUp->IsA<APlayerCharacter>())
	{
		PlayerCharacter = Cast<APlayerCharacter>(ActorThatPickedUp);

		if (!PlayerCharacter->bJumpPickedUp)		//checks if the player hasn't already picked up a jump powerup
		{
			PlayerCharacter->GetCharacterMovement()->JumpZVelocity = PlayerCharacter->PickupJumpHeight;

			PlayerCharacter->IncreaseJump();

			PlayerCharacter->bJumpPickedUp = true;		//indicates that the player has picked up a jump powerup to prevent them from picking up multiple until the timer has finished

			Destroy();
		}
	}
}
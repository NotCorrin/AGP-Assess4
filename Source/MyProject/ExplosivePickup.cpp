// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosivePickup.h"

void AExplosivePickup::OnGenerate()
{
	APickup::OnGenerate();
}

void AExplosivePickup::OnPickup(AActor* ActorThatPickedUp)
{
	APickup::OnPickup(ActorThatPickedUp);
}
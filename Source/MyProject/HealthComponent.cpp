// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/GameEngine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.0f;

	TakeDamage = 0;
	MaxArmor = 0;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GEngine && GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), CurrentHealth));
	}
}

void UHealthComponent::OnTakeDamage(float Damage)
{

	if (TakeDamage > 0)	//checks if an armor powerup has been picked up
	{
		TakeDamage -= 1;	//remove 1 from TakeDamage every time the parent actor is shot
	}
	else {

		CurrentHealth -= Damage;

		if (CurrentHealth < 0.0f)
		{
			CurrentHealth = 0;
			OnDeath();
		}
	}
}

void UHealthComponent::OnDeath()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnDeath();

		UE_LOG(LogTemp, Error, TEXT("Dead"));
	}
}

float UHealthComponent::HealthPercentageRemaining()
{
	return CurrentHealth / MaxHealth * 100.0f;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, TakeDamage);
}

void UHealthComponent::UpdateHealthBar()
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		AHUD* HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();

		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(HUD);

		if (PlayerHUD != nullptr)
		{
			PlayerHUD->SetPlayerHealthBarPercent(HealthPercentageRemaining() / 100.0f);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIManager.h"
#include "NavigationNode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "HealthComponent.h"
#include "TimerManager.h"
#include "Engine/GameEngine.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentAgentState = AgentState::PATROL;

	PathdfindingNodeAccuracy = 100.0f;

	bHOTPickedUp = false;
	bWeaponShotTypePickedUp = false;

	HOTTimer = 0;
	HOTEndTimer = 0;
	ShotTypeTimer = 0;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Cast<UCharacterMovementComponent>(GetMovementComponent())->bOrientRotationToMovement = true;

	HealthComponent = FindComponentByClass<UHealthComponent>();

	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);
	}

	DetectedActor = nullptr;
	bCanSeeActor = false;

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAgentState == AgentState::PATROL)
	{
		AgentPatrol();
		if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 40.0f)
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	else if (CurrentAgentState == AgentState::ENGAGE)
	{
		AgentEngage();
		if (!bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 40.0f)
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	else if (CurrentAgentState == AgentState::EVADE)
	{
		AgentEvade();
		if (!bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
	}

	MoveAlongPath();

	if (bHOTPickedUp && HOTTimer == 0)	//checks if the enemy has picked up a HOT power up and if the HOTTimer is set to 0
	{
		HOTTimer = 1;	//sets timer to 1 so the HOTPickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemyCharacter::HOTPickup, 1.f, true);	//calls and loops the HOTPickup function every 1 seconds

		HOTEndTimer = 0;	//after all health points have been added resets timer fo next pick up
	}
	else if (bWeaponShotTypePickedUp && ShotTypeTimer == 0)		//checks if the weapon shot type power up has been picked up and if the ShotTypeTimer is set to 0
	{
		ShotTypeTimer = 1;	//sets timer to 1 so the RemoveWeaponShotTypePickup function isn't called every frame

		FTimerHandle MemberTimerHandle;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemyCharacter::RemoveWeaponShotType, 10.f, true);	//calls the RemoveWeaponShotTypePickup function after 10 seconds
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::AgentPatrol()
{
	if (Path.Num() == 0)
	{
		if (Manager)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
		}
	}
}

void AEnemyCharacter::AgentEngage()
{
	if (bCanSeeActor && DetectedActor)
	{
		FVector FireDirection = DetectedActor->GetActorLocation() - GetActorLocation();
		Fire(FireDirection);
	}
	if (Path.Num() == 0 && DetectedActor)
	{
		ANavigationNode* NearestNode = Manager->FindNearestNode(DetectedActor->GetActorLocation());
		Path = Manager->GeneratePath(CurrentNode, NearestNode);
	}
}

void AEnemyCharacter::AgentEvade()
{
	if (bCanSeeActor && DetectedActor)
	{
		FVector FireDirection = DetectedActor->GetActorLocation() - GetActorLocation();
		Fire(FireDirection);
	}
	if (Path.Num() == 0 && DetectedActor)
	{
		ANavigationNode* FurthestNode = Manager->FindFurthestNode(DetectedActor->GetActorLocation());
		Path = Manager->GeneratePath(CurrentNode, FurthestNode);
	}
}

void AEnemyCharacter::SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
			DetectedActor = ActorSensed;
		bCanSeeActor = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
			bCanSeeActor = false;
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(PathdfindingNodeAccuracy)
		&& Path.Num() > 0)
	{
		CurrentNode = Path.Pop();
	}
	else if (!(GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(PathdfindingNodeAccuracy))
	{
		AddMovementInput(CurrentNode->GetActorLocation() - GetActorLocation());
	}
}

void AEnemyCharacter::HOTPickup()
{
	if (HealthComponent->CurrentHealth < HealthComponent->MaxHealth)	//checks if the enemy's Current Health is less than its Max Health
	{
		HOTEndTimer += 1;

		if (HOTEndTimer <= 6.0f)	//checks if it has been less than 6 seconds
		{
			HealthComponent->CurrentHealth += 5.0f;		//increases player's Current Health

			if (HealthComponent->CurrentHealth > HealthComponent->MaxHealth)	//checks if after the increase, the enemy's Currrent Health is greater than its Max Health
			{
				HealthComponent->CurrentHealth = HealthComponent->MaxHealth;	//Makes the enemy's Current Health equal to its Max Health
			}

			GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Red, FString::Printf(TEXT("Current Enemy Health: %f"), HealthComponent->CurrentHealth));	//prints its Current Health to the screen
		}
		else
		{
			bHOTPickedUp = false;	//resets for next pick up

			HOTTimer = 0;	//resets for next pick up
		}
	}
}

void AEnemyCharacter::RemoveWeaponShotType()
{
	bWeaponShotTypePickedUp = false;	//resets for next pick up

	WeaponShotType = EEnemyWeaponShotType::SINGLE_SHOT;		//returns shot type back to original value

	ShotTypeTimer = 0;	//resets for next pick up
}
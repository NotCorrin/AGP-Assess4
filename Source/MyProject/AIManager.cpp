// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "EngineUtils.h"
#include "NavigationNode.h"
#include "EnemyCharacter.h"
#include "Pickup.h"
#include "TimerManager.h"

// Sets default values
AAIManager::AAIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AllowedAngle = 0.4f;
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (AllNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("POPULATING NODES"))
		FTimerHandle PopulateTimerHandle;
		GetWorldTimerManager().SetTimer(PopulateTimerHandle, this, &AAIManager::PopulateNodes, 4, false);
	}

	FTimerHandle AgentTimerHandle;
	GetWorldTimerManager().SetTimer(AgentTimerHandle, this, &AAIManager::CreateAgents, 5, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Number of nodes: %i"), AllNodes.Num())
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<ANavigationNode*> AAIManager::GeneratePath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
	// Create the open set array and add the start node
	TArray<ANavigationNode*> OpenSet;
	OpenSet.Add(StartNode);

	// Set all the GScores to infinity
	for (auto It = AllNodes.CreateIterator(); It; ++It)
	{
		(*It)->GScore = TNumericLimits<float>::Max();
	}

	// Set start node GScore to zero and update the heuristic
	StartNode->GScore = 0.0f;
	StartNode->HScore = FVector::Dist(StartNode->GetActorLocation(), EndNode->GetActorLocation());

	ANavigationNode* CurrentNode;

	// Loop through the open set until it is empty
	while (OpenSet.Num() > 0)
	{
		// Find the node with the lowest FScore
		ANavigationNode* BestNode = OpenSet[0];
		for (auto It = OpenSet.CreateConstIterator(); It; ++It)
		{
			// If the best node is the current node then skip
			if (*It == BestNode) continue;
			// If the current nodes fscore is better than the 
			// best nodes fscore then set the current node as the best node
			if ((*It)->FScore() < BestNode->FScore())
			{
				BestNode = *It;
			}
		}
		// When this loop finishes, the BestNode will be the node with the lowest FScore in the open set
		CurrentNode = BestNode;
		OpenSet.Remove(CurrentNode);

		// If the current node is the end node then we have the path and should reconstruct it
		if (CurrentNode == EndNode)
		{
			return ReconstructPath(StartNode, EndNode);
		}

		// Loop through all of the connected nodes of the current node
		for (auto It = CurrentNode->ConnectedNodes.CreateIterator(); It; ++It)
		{
			float TentativeGScore = CurrentNode->GScore + FVector::Dist(CurrentNode->GetActorLocation(), (*It)->GetActorLocation());
			if (TentativeGScore < (*It)->GScore)
			{
				(*It)->CameFrom = CurrentNode;
				(*It)->GScore = TentativeGScore;
				(*It)->HScore = FVector::Dist((*It)->GetActorLocation(), EndNode->GetActorLocation());
				if (!OpenSet.Contains(*It))
				{
					OpenSet.Add(*It);
				}
			}
		}
	}

	// If it exists this loop then no valid path has been found so return an empty path.

	return TArray<ANavigationNode*>();
}

TArray<ANavigationNode*> AAIManager::ReconstructPath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
	TArray<ANavigationNode*> Path;
	ANavigationNode* CurrentNode = EndNode;
	while (CurrentNode != StartNode)
	{
		Path.Add(CurrentNode);
		CurrentNode = CurrentNode->CameFrom;
	}
	return Path;
}

void AAIManager::PopulateNodes()
{
	UE_LOG(LogTemp, Error, TEXT("Populate nodes function is called"))
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		UE_LOG(LogTemp, Error, TEXT("POPULATING!!"))
		AllNodes.Add(*It);

		AllPickupNodes.Add(*It);	//adds all navigation nodes to an array
	}
}

void AAIManager::CreateAgents()
{
	UE_LOG(LogTemp, Error, TEXT("Create Agents is running"))
	if (AllNodes.Num() > 0)
	{
		for (int32 i = 0; i < NumAI; i++)
		{
			// Get a random node index
			int32 NodeIndex = FMath::RandRange(0, AllNodes.Num() - 1);
			AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(AgentToSpawn, AllNodes[NodeIndex]->GetActorLocation(), AllNodes[NodeIndex]->GetActorRotation());
			SpawnedEnemy->Manager = this;
			SpawnedEnemy->CurrentNode = AllNodes[NodeIndex];
		}
	}
}

ANavigationNode* AAIManager::FindNearestNode(const FVector& Location)
{
	ANavigationNode* NearestNode = nullptr;
	float NearestDistance = TNumericLimits<float>::Max();
	//Loop through the nodes and find the nearest one in distance
	for (ANavigationNode* CurrentNode : AllNodes)
	{
		float CurrentNodeDistance = FVector::Distance(Location, CurrentNode->GetActorLocation());
		if (CurrentNodeDistance < NearestDistance)
		{
			NearestDistance = CurrentNodeDistance;
			NearestNode = CurrentNode;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Nearest Node: %s"), *NearestNode->GetName())
		return NearestNode;
}

ANavigationNode* AAIManager::FindFurthestNode(const FVector& Location)
{
	ANavigationNode* FurthestNode = nullptr;
	float FurthestDistance = 0.0f;
	//Loop through the nodes and find the nearest one in distance
	for (ANavigationNode* CurrentNode : AllNodes)
	{
		float CurrentNodeDistance = FVector::Distance(Location, CurrentNode->GetActorLocation());
		if (CurrentNodeDistance > FurthestDistance)
		{
			FurthestDistance = CurrentNodeDistance;
			FurthestNode = CurrentNode;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Furthest Node: %s"), *FurthestNode->GetName())
		return FurthestNode;
}
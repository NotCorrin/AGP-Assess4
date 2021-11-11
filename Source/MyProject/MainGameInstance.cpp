// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "UObject/ConstructorHelpers.h"

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitialize)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetObject(TEXT("/Game/Widgets/GameOverWidget"));

	GameOverWidgetClass = GameOverWidgetObject.Class;
}

void UMainGameInstance::LoadMenu()
{
	if (GameOverWidgetClass)
	{
		GameOver = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidgetClass);

		if (GameOver)
		{
			GameOver->AddToViewport();
		}
	}
}
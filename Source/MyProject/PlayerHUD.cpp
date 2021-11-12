// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDObject(TEXT("/Game/Widgets/PlayerHUDWidget"));

	PlayerHUDClass = PlayerHUDObject.Class;

	if (PlayerHUDClass)
	{
		CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

		if (CurrentPlayerHUDWidget)
		{
			CurrentPlayerHUDWidget->AddToViewport();	//adds player HUD to viewport

			HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
			RoundsTitle = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsTitle")));
			RoundsRemainingText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsRemaining")));
			ArmorTitle = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ArmorTitle")));
			ArmorText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Armor")));
			GameOverText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("GameOver")));
			BackgroundImage = Cast<UImage>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Background")));

			if (GameOverText)
			{
				GameOverText->SetOpacity(0.0f);		//makes the Game Over text not visible on the HUD
			}
			if (BackgroundImage)
			{
				BackgroundImage->SetOpacity(0.0f);	//makes the background not visible on the HUD
			}
		}
	}
}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);	//sets health progress bar on the HUD to be the players current health
	}
}

void APlayerHUD::SetAmmoText(int32 RoundsRemaining)
{
	if (RoundsRemainingText)
	{
		RoundsRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%i"), RoundsRemaining)));	//shows how many bullets a player has on their HUD
	}
}

void APlayerHUD::SetArmorText(int32 TakeDamage)
{
	if (ArmorText)
	{
		ArmorText->SetText(FText::FromString(FString::Printf(TEXT("%i"), TakeDamage)));		//shows how much armor a player has on their HUD
	}
}

void APlayerHUD::SetGameOver()
{
	GameOverText->SetOpacity(1.0f);		//shows Game Over text on the player's HUD
	BackgroundImage->SetOpacity(1.0f);	//shows background on the player's HUD

	// Removes the following HUD elements
	HealthProgressBar->SetRenderOpacity(0.0f);
	RoundsTitle->SetOpacity(0.0f);
	RoundsRemainingText->SetOpacity(0.0f);
	ArmorTitle->SetOpacity(0.0f);
	ArmorText->SetOpacity(0.0f);
}
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

	if (PlayerHUDClass != nullptr)
	{

		CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

		if (CurrentPlayerHUDWidget != nullptr)
		{
			CurrentPlayerHUDWidget->AddToViewport();

			HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
			RoundsRemainingText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsRemaining")));
			ArmorText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Armor")));
			GameOverText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("GameOver")));
			BackgroundImage = Cast<UImage>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Background")));

			if (GameOverText)
			{
				GameOverText->SetOpacity(0.0f);
			}

			if (BackgroundImage)
			{
				BackgroundImage->SetOpacity(0.0f);
			}
		}
	}
}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
	if (HealthProgressBar != nullptr)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void APlayerHUD::SetAmmoText(int32 RoundsRemaining)
{
	if (RoundsRemainingText)
	{
		RoundsRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%i"), RoundsRemaining)));
	}
}

void APlayerHUD::SetArmorText(int32 TakeDamage)
{
	if (ArmorText)
	{
		ArmorText->SetText(FText::FromString(FString::Printf(TEXT("%i"), TakeDamage)));
	}
}

void APlayerHUD::SetGameOver()
{
	GameOverText->SetOpacity(1.0f);
	BackgroundImage->SetOpacity(1.0f);

	HealthProgressBar->SetRenderOpacity(0.0f);
	RoundsRemainingText->SetOpacity(0.0f);
	ArmorText->SetOpacity(0.0f);
}
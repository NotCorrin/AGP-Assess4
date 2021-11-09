// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"

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

			UWidget* ProgressBar = CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar"));

			HealthProgressBar = Cast<UProgressBar>(ProgressBar);
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
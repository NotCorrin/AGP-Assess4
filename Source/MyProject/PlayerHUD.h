// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	APlayerHUD();

	void SetPlayerHealthBarPercent(float Percent);

private:

	TSubclassOf<UUserWidget> PlayerHUDClass;
	UUserWidget* CurrentPlayerHUDWidget;

	UProgressBar* HealthProgressBar;

};
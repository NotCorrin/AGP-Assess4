// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

	void SetGameOver();

private:

	TSubclassOf<class UUserWidget> PlayerHUDClass;
	UUserWidget* CurrentPlayerHUDWidget;
	class UProgressBar* HealthProgressBar;
	class UTextBlock* RoundsTitle;
	UTextBlock* RoundsRemainingText;
	UTextBlock* ArmorTitle;
	UTextBlock* ArmorText;
	UTextBlock* GameOverText;
	class UImage* BackgroundImage;

	UFUNCTION(BlueprintCallable)
	void SetAmmoText(int32 RoundsRemaining);

	UFUNCTION(BlueprintCallable)
	void SetArmorText(int32 TakeDamage);

};
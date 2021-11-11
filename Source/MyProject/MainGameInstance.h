// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameOverWidget.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UMainGameInstance(const FObjectInitializer& ObjectInitialize);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

private:

	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UGameOverWidget* GameOver;
};

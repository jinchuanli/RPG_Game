// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartGameHUD.generated.h"

class UStartGameWidget;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AStartGameHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UStartGameWidget> StartGameWidget;

	UStartGameWidget* StartMain;
public:
	AStartGameHUD();

	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSave.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API URPGSave : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FString SavedName;
	UPROPERTY()
	float SavedHP;
	UPROPERTY()
	float SaveMP;
	UPROPERTY()
	float SaveEXP;
	UPROPERTY()
	float SavedLevel;
	UPROPERTY()
	float SavedCoin;
};

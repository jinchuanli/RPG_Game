// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DREAMHUD.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ADREAMHUD : public AHUD
{
	GENERATED_BODY()

	void BeginPlay() override;
	class ARBaseCharacter* BaseCharacter;
	class UMainUserWidget* MainUserWidget;
};

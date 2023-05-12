// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "RBaseCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ABaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

	ABaseGameModeBase();
public:
	
	void Respawn(ABasePlayerController* Controller);

	ARBaseCharacter* CharacterRef;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BaseNPC.h"
#include "SyntheticSystem.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ASyntheticSystem : public ABaseNPC
{
	GENERATED_BODY()
	
public:
	class ARBaseCharacter* PlayerCharacter;
	class UCraftingMenuWidget* CraftingMenuWidget;

	virtual void BeginPlay() override;

	virtual void OnIteractWith(ARBaseCharacter* Character) override;

	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
	
};

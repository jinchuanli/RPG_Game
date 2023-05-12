// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API URecipeEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;

public:
	TSubclassOf<class ABaseItem> Item;
	class UCraftingMenuWidget* CraftingMenu;
	int RequiredAmount;
	int CurrentAmount;

	void Update();
};

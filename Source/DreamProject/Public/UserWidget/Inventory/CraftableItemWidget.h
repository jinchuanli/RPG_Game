// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UCraftableItemWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class UCraftingMenuWidget;

	UPROPERTY(meta=(BindWidget))
	class UButton* SelectedButton;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameText;

	TSubclassOf<class ABaseItem> Item;
	class UCraftingMenuWidget* CraftingMenu;

	UFUNCTION()
	void OnSelectButtonClicked();

	virtual void NativeConstruct() override;
public:
	void Init(TSubclassOf<class ABaseItem> FItem,UCraftingMenuWidget* FCraftingMenu);
};

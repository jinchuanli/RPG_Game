// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/BaseItem.h"
#include "CraftingMenuWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UScrollBox;
class UVerticalBox;
class UBorder;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DescriptionText;
	UPROPERTY(meta=(BindWidget))
	UButton* CraftButton;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* CraftableItemScrollBox;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* RecipeEntriesList;
	UPROPERTY(meta=(BindWidget))
	UBorder* DetailBorder;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnCraftButtonClicked();  //合成按钮的点击

public:
	class AInventory* Inventory;
	void OnCraftableCliked(class UCraftableItemWidget* ClickedItem);

	void GenerateCraftableList();

	class UCraftableItemWidget* CurrentCraftable;
	TSubclassOf<ABaseItem> CurrentItem;
	FItemInfo CurrentItemInfo;
	TArray<class URecipeEntryWidget*> RecipeEntries;
	void GenerateRecipeEntries();

	//能否合成
	bool bCanBeCrafted();

	//更新合成窗口
	void UpdateDetailWindow(TSubclassOf<class ABaseItem> Item);

	//初始化合成界面
	void initCraftingMenu(class AInventory* FInventory);
	
};

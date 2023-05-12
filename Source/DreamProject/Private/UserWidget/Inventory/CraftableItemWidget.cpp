// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/CraftableItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"

void UCraftableItemWidget::OnSelectButtonClicked()
{
	CraftingMenu->OnCraftableCliked(this);
}

void UCraftableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectedButton->OnClicked.AddDynamic(this,&UCraftableItemWidget::OnSelectButtonClicked);
}

void UCraftableItemWidget::Init(TSubclassOf<ABaseItem> FItem, UCraftingMenuWidget* FCraftingMenu)
{
	this->Item = FItem;
	this->CraftingMenu = FCraftingMenu;
	NameText->SetText(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Name);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/CraftingMenuWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/CraftableItemWidget.h"
#include "UserWidget/Inventory/RecipeEntryWidget.h"

void UCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UCraftingMenuWidget::OnCloseButtonClicked);
	CraftButton->OnClicked.AddDynamic(this,&UCraftingMenuWidget::OnCraftButtonClicked);
}

void UCraftingMenuWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	Inventory->PlayerCharacter->PC->bEnableClickEvents = false;
	Inventory->PlayerCharacter->PC->bShowMouseCursor = false;
	Inventory->PlayerCharacter->PC->SetInputMode(InputMode);
}

void UCraftingMenuWidget::OnCraftButtonClicked()
{
	if(Inventory->AddItem(CurrentItem,1) == 0)
	{
		for(FInventorySlot FSlot : CurrentItemInfo.Recipe)
		{
			Inventory->RemoveItem(FSlot.ItemClass,FSlot.Amount);
		}
	}
	CraftButton->SetIsEnabled(bCanBeCrafted());
}

void UCraftingMenuWidget::OnCraftableCliked(UCraftableItemWidget* ClickedItem)
{
	if(CurrentCraftable)
	{
		CurrentCraftable->SelectedButton->SetIsEnabled(true);
		CurrentCraftable = ClickedItem;
		CurrentCraftable->SelectedButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftable->Item);
	}
	else
	{
		CurrentCraftable = ClickedItem;
		CurrentCraftable->SelectedButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftable->Item);
	}
}

void UCraftingMenuWidget::GenerateCraftableList()
{
	CraftableItemScrollBox->ClearChildren();
	for(TSubclassOf<ABaseItem> Item : Inventory->CraftableItems)
	{
		UCraftableItemWidget* CraftableItemWidget = CreateWidget<UCraftableItemWidget>(GetWorld(),
			LoadClass<UCraftableItemWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_CraftableItem.WBP_CraftableItem_C'")));
		CraftableItemWidget->Init(Item,this);
		CraftableItemScrollBox->AddChild(CraftableItemWidget);
	}
}

void UCraftingMenuWidget::GenerateRecipeEntries()
{
	RecipeEntriesList->ClearChildren();
	RecipeEntries.Empty();
	for(FInventorySlot FSlot : CurrentItemInfo.Recipe)
	{
		URecipeEntryWidget* RecipeEntryWidget = CreateWidget<URecipeEntryWidget>(GetWorld(),
			LoadClass<URecipeEntryWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_RecipeEntry.WBP_RecipeEntry_C'")));
		RecipeEntryWidget->Item = FSlot.ItemClass;
		RecipeEntryWidget->RequiredAmount = FSlot.Amount;
		RecipeEntryWidget->CraftingMenu = this;
		RecipeEntryWidget->Update();
		RecipeEntries.Add(RecipeEntryWidget);
		RecipeEntriesList->AddChild(RecipeEntryWidget);
	}
}

bool UCraftingMenuWidget::bCanBeCrafted()
{
	bool TempBool = true;
	for(URecipeEntryWidget* Recipe : RecipeEntries)
	{
		if(Recipe->CurrentAmount < Recipe->RequiredAmount)
		{
			TempBool = false;
			break;
		}
	}
	return TempBool;
}

void UCraftingMenuWidget::UpdateDetailWindow(TSubclassOf<ABaseItem> Item)
{
	if(Item)
	{
		if(Item == CurrentItem)
		{
			for(URecipeEntryWidget* Recipe : RecipeEntries)
			{
				Recipe->Update();
			}
			CraftButton->SetIsEnabled(bCanBeCrafted());
			DetailBorder->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CurrentItem = Item;
			CurrentItemInfo = Item->GetDefaultObject<ABaseItem>()->ItemInfo;
			IconImage->SetBrushFromTexture(CurrentItemInfo.Icon);
			NameText->SetText(CurrentItemInfo.Name);
			DescriptionText->SetText(CurrentItemInfo.Description);
			GenerateRecipeEntries();
			CraftButton->SetIsEnabled(bCanBeCrafted());
			DetailBorder->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		CurrentItem = nullptr;
		RecipeEntriesList->ClearChildren();
		DetailBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCraftingMenuWidget::initCraftingMenu(AInventory* FInventory)
{
	this->Inventory = FInventory;
	GenerateCraftableList();
	UpdateDetailWindow(nullptr);
}



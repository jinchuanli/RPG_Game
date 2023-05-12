// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/RecipeEntryWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"

#define LOCTEXT_NAMESPACE "RecipeEntryWidget"

void URecipeEntryWidget::Update()
{
	IconImage->SetBrushFromTexture(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Icon);
	TArray<int> ItemIndices;
	CurrentAmount = CraftingMenu->Inventory->GetTotalAmountOfItem(Item,ItemIndices);
	NameText->SetText(FText::Format(LOCTEXT("RecipeEntryWidget","{0}({1}/{2})"),
		Item->GetDefaultObject<ABaseItem>()->ItemInfo.Name,
		FText::AsNumber(CurrentAmount),
		FText::AsNumber(RequiredAmount)));
	if(CurrentAmount >= RequiredAmount)
		NameText->SetColorAndOpacity(FLinearColor::White);
	else
	{
		NameText->SetColorAndOpacity(FLinearColor::Red);
	}
}

#undef LOCTEXT_NAMESPACE

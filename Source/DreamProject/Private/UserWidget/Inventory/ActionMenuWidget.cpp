// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ActionMenuWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"

void UActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UseItemButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnUseButtonClicked);
	ThrowAwayButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnThrowButtonClicked);
	SplitStacksbutton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnSplitButtonClicked);
	CancelButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnCancelButtonClicked);
}

void UActionMenuWidget::Update(int Index)
{
	CurrentIndex = Index;
	ItemInfo = InventoryRef->GetItemAtIndex(Index)->ItemInfo;
	Amount = InventoryRef->GetAmountAtIndex(Index);

	if(ItemInfo.bCanBeUsed)
	{
		UseText->SetText(ItemInfo.UseText);
		UseItemButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UseItemButton->SetVisibility(ESlateVisibility::Collapsed);//不占用局部空间
	}
	if(Amount > 1 && ItemInfo.bCanBeStacked)
	{
		SplitStacksbutton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SplitStacksbutton->SetVisibility(ESlateVisibility::Collapsed);//不占用局部空间
	}
}

void UActionMenuWidget::OnUseButtonClicked()
{
	InventoryRef->UseItemAtIndex(CurrentIndex);
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnThrowButtonClicked()
{
	if(!ItemInfo.bCanBeStacked && Amount > 0)
	{
		InventoryRef->RemoveItemAtIndex(CurrentIndex,1);
	}
	else
	{
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->Update(CurrentIndex);
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
	}
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnSplitButtonClicked()
{
	InventoryRef->SplitStacke(CurrentIndex,int(Amount/2));
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

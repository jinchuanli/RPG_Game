// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ShopWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/Merchant.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "UserWidget/Inventory/SellWidget.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UShopWidget::OnCloseButtonClicked);
}

void UShopWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	PlayerCharacter->PC->bEnableClickEvents = false;
	PlayerCharacter->PC->bShowMouseCursor = false;
	PlayerCharacter->PC->SetInputMode(InputMode);
	Merchant->bShopOpen = false;
}

bool UShopWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if(Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* slot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if(slot->ItemInfo.Category == EItemCategories::QuestItem)
		{
			return false;
		}
		else
		{
			if(!slot->ItemInfo.bCanBeStacked && slot->Amount > 0)
			{
				if(PlayerCharacter->InventoryRef->RemoveItemAtIndex(slot->SlotIndex,1))
				{
					PlayerCharacter->IncreaseCoin(slot->ItemInfo.Price);
					UpdateAllItems();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				SellWidget->Update(slot->SlotIndex);
				SellWidget->SetVisibility(ESlateVisibility::Visible);
				PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
				this->GridPanel->SetIsEnabled(false);
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

void UShopWidget::UpdateCoin()
{
	CoinText->SetText(FText::AsNumber(PlayerCharacter->CurrentCoin));
}

void UShopWidget::GenerateItemList()
{
	OfferedItemWidgets.Empty();
	GridPanel->ClearChildren();
	for(int i = 0;i < Merchant->OfferendItems.Num();i++)
	{
		UOfferedItemWidget* OfferedItemWidget = CreateWidget<UOfferedItemWidget>(GetWorld(),
			LoadClass<UOfferedItemWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/Merchant/WBP_OfferedItem.WBP_OfferedItem_C'")));
		OfferedItemWidget->Init(Merchant->OfferendItems[i],PlayerCharacter->InventoryRef,this);
		OfferedItemWidgets.Add(OfferedItemWidget);
		UPanelSlot* slot = GridPanel->AddChild(OfferedItemWidget);
		Cast<UUniformGridSlot>(slot)->SetRow(i / 2);
		Cast<UUniformGridSlot>(slot)->SetColumn(i % 2);
	}
}

void UShopWidget::UpdateAllItems()
{
	for(UOfferedItemWidget* ItemWidget : OfferedItemWidgets)
	{
		ItemWidget->UpdatePrice();
	}
}

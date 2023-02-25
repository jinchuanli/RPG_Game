// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/InventorySlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"

void UInventorySlotWidget::UpdateSlot()
{
	if(InventoryRef->bIsSlotEmpty(SlotIndex))  //为空，不催在
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	else  // 存在
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = InventoryRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = InventoryRef->Slots[SlotIndex].Amount;
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
		IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);  //显示但是不能点击
		if(ItemInfo.bCanBeStacked)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

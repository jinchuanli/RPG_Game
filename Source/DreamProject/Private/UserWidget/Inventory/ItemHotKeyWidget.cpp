// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemHotKeyWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"

void UItemHotKeyWidget::Update()
{
	if(bEmpty)
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		bDraggedOver = false;
		Bg->SetBrushColor(DefaultTint);
	}
	else
	{
		ABaseItem* Item= Inventory->GetItemAtIndex(InventoryIndex);
		if(Item == nullptr)
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
			AmountText->SetVisibility(ESlateVisibility::Hidden);
			bDraggedOver = false;
			Bg->SetBrushColor(DefaultTint);
		}
		else
		{
			IconImage->SetBrushFromTexture(Item->ItemInfo.Icon);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			AmountText->SetText(FText::AsNumber(Inventory->Slots[InventoryIndex].Amount));
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}

FReply UItemHotKeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !bEmpty)
	{
		bEmpty = true;
		Update();
	}
	return FReply::Handled();
}

bool UItemHotKeyWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		return true;
	}else if(Cast<UItemDragDropOperation>(InOperation))
	{
		bDraggedOver = true;
		Bg->SetBrushColor(DragOverTint);
		return true;
	}
	else
	{
		return false;
	}
}

void UItemHotKeyWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		if(Cast<UItemDragDropOperation>(InOperation))
		{
			Bg->SetBrushColor(DefaultTint);
			bDraggedOver = false;
		}
	}
}

bool UItemHotKeyWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if(Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* FSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		InventoryIndex = FSlot->SlotIndex;
		Inventory = FSlot->InventoryRef;
		bEmpty = false;
		Update();
		bDraggedOver = false;
		Bg->SetBrushColor(DefaultTint);
		return true;
	}
	else
	{
		return false;
	}
}

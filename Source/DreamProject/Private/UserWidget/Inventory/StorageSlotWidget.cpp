// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "MovieSceneClipboard.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ItemDetailWidget.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "UserWidget/Inventory/ItemDragWidget.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/Inventory/StorageSlotDragDropOperation.h"
#include "UserWidget/Inventory/StorageWidget.h"

void UStorageSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this,&UStorageSlotWidget::OnSlotButtonClicked);
}

void UStorageSlotWidget::UpdateSlot()
{
	if(StorageRef->bIsSlotEmpty(SlotIndex))  //为空，不催在
		{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);
		}
	else  // 存在
		{
		SlotButton->SetIsEnabled(true);
		ItemInfo = StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = StorageRef->GetAmountAtIndex(SlotIndex);
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
		if(!ItemDetail)
		{
			ItemDetail = CreateWidget<UItemDetailWidget>(GetWorld(),
				LoadClass<UItemDetailWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemDetail.WBP_ItemDetail_C'")));
		}
		ItemDetail->ItemInfo = ItemInfo;
		ItemDetail->Amount = Amount;
		SlotButton->SetToolTip(ItemDetail);
		ItemDetail->Update();
		}
}


void UStorageSlotWidget::OnSlotButtonClicked()
{
	StorageWidget->OnSlotClicked(SlotIndex);
}

FReply UStorageSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown"));
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}

void UStorageSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if(!StorageRef->bIsSlotEmpty(SlotIndex))
	{
		//设置拖拽的图标
		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(),
		   LoadClass<UItemDragWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));
		ItemDrag->Init(Amount,StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);

		UDragDropOperation* StorageSlotOp = UWidgetBlueprintLibrary::CreateDragDropOperation(StorageSlotDrag);
		StorageSlotOp->DefaultDragVisual = ItemDrag;
		OutOperation = StorageSlotOp;

		if(Cast<UStorageSlotDragDropOperation>(OutOperation))
		{
			Cast<UStorageSlotDragDropOperation>(OutOperation)->Slot = this;
		}
	}
}

bool UStorageSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		return true;
	}
	else
	{
		if(Cast<UItemDragDropOperation>(InOperation))
		{
			bDraggedOver = true;
			Base->SetBrushColor(FLinearColor(1,0.82,0,0.5));
			return true;
		}
		else if(Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			if(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot != this)
			{
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1,0.82,0,0.5));
				return true;
			}
		}
		return false;
	}
}

void UStorageSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		if(Cast<UItemDragDropOperation>(InOperation) || Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1,1,1,0));
		}
	}
}

bool UStorageSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if(Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		if(StorageRef->AddToIndex(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex,SlotIndex))
		{
			return true;
		}
		else
		{
			return StorageRef->SwapSlot(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex,SlotIndex);
		}
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1,1,1,0));
	}
	else if(Cast<UItemDragDropOperation>(InOperation))
	{
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1,1,1,0));
		return StorageRef->MoveFromInventoryToStorageByIndex(Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->InventoryRef,Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->SlotIndex,SlotIndex);
	}
	else
	{
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1,1,1,0));
		return false;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


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
#include "UserWidget/Inventory/StorageSlotDragDropOperation.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this,&UInventorySlotWidget::OnSlotButtonClicked);
}

void UInventorySlotWidget::UpdateSlot()
{
	if(InventoryRef->bIsSlotEmpty(SlotIndex))  //为空，不催在
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);
	}
	else  // 存在
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = InventoryRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = InventoryRef->GetAmountAtIndex(SlotIndex);
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

void UInventorySlotWidget::SetCountToZero()
{
	clickCount = 0;
	InventoryRef->UpdateActionMenuPosition(this);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}

void UInventorySlotWidget::OnSlotButtonClicked()
{
	clickCount++;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero,this,&UInventorySlotWidget::SetCountToZero,0.3f,false);
	if(clickCount > 1)
	{
		InventoryRef->UseItemAtIndex(SlotIndex);
		clickCount = 0;
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
	}
}

// FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
// 	const FPointerEvent& InMouseEvent)
// {
// 	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
// 	UE_LOG(LogTemp, Warning, TEXT("双击使用物品"));
// 	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && ItemInfo.bCanBeUsed)
// 	{
// 		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Hidden);
// 		UE_LOG(LogTemp, Warning, TEXT("鼠标左键双击使用物品"));
// 		InventoryRef->UseItemAtIndex(SlotIndex);
// 		return FReply::Handled();
// 	}
// 	else
// 	{
// 		return FReply::Handled();
// 	}
// }

// FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
// 	const FPointerEvent& InMouseEvent)
// {
// 	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
// 	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
// 	{
// 		if(!InventoryRef->bIsSlotEmpty(SlotIndex))
// 			InventoryRef->UpdateActionMenuPosition(this);
// 		return FReply::Handled();
// 	}
// 	else if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown"));
// 		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
// 	}
// 	else
// 	{
// 		return FReply::Handled();
// 	}
// }

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if(!InventoryRef->bIsSlotEmpty(SlotIndex))
	{
		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(),
		   LoadClass<UItemDragWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));
		ItemDrag->Init(Amount,InventoryRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);
		UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(ItemDragOperation);
		TempOp->DefaultDragVisual = ItemDrag;
		OutOperation = TempOp;

		if(Cast<UItemDragDropOperation>(OutOperation))
		{
			Cast<UItemDragDropOperation>(OutOperation)->bAltDown = InventoryRef->PlayerCharacter->bAltDown;
			Cast<UItemDragDropOperation>(OutOperation)->DraggedSlot = this;
		}
	}
}

bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
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
			if(Cast<UItemDragDropOperation>(InOperation)->DraggedSlot != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("UInventorySlotWidget::NativeOnDragOver"));
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1.f,0.82f,0.f,0.5f));
			}
			return true;
		}
		else
		{
			if(Cast<UStorageSlotDragDropOperation>(InOperation))
			{
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1.f,0.82f,0.f,0.5f));
			}
			return true;
		}
	}
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		if(Cast<UItemDragDropOperation>(InOperation) || Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			Base->SetBrushColor(FLinearColor(1,1,1,0));
			bDraggedOver = false;
		}
	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if(Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* FSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if(FSlot != this)
		{
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1,1,1,0));
			if(InventoryRef->AddToIndex(FSlot->SlotIndex,SlotIndex))
			{
				return true;
				UE_LOG(LogTemp, Warning, TEXT("InventoryRef->AddToIndex"));
			}
			else
			{
				if(Cast<UItemDragDropOperation>(InOperation)->bAltDown)
				{
					InventoryRef->SplitStackToIndex(FSlot->SlotIndex,SlotIndex,FSlot->Amount/2);
					UE_LOG(LogTemp, Warning, TEXT("SplitStackToIndex"));
				}
				else
				{
					InventoryRef->SwapSlot(FSlot->SlotIndex,SlotIndex);
				}
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else if(Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		return InventoryRef->MoveFromStorageToInventoryByIndex(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->StorageRef,Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex,SlotIndex);
	}
	else
	{
		return false;
	}
}

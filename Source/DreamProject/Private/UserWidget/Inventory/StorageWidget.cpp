// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/StorageWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "Other/StaticLibrary.h"
#include "UserWidget/Inventory/InventoryDragDropOperation.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/Inventory/StorageSlotWidget.h"

void UStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UStorageWidget::OnCloseButtonClicked);
}

void UStorageWidget::GenerateSlotWidgets(class AStorage* FStorageRef)
{
	if(StorageRef == nullptr)
	{
		StorageRef = FStorageRef;
	}
	else
	{
		if(FStorageRef == this->StorageRef)
		{
			return;
		}
		else
		{
			StorageRef = FStorageRef;
		}
	}
	StorageBox->ClearChildren();
	StorageSlotWidgets.Empty();
	for(int i = 0; i < StorageRef->Slots.Num();i++)
	{
		UStorageSlotWidget* StorageSlotWidget = CreateWidget<UStorageSlotWidget>(GetWorld(),
			LoadClass<UStorageSlotWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_StorageSlot.WBP_StorageSlot_C'")));
		StorageSlotWidget->StorageRef = StorageRef;
		StorageSlotWidget->SlotIndex = i;
		StorageSlotWidget->StorageWidget = this;
		StorageSlotWidget->UpdateSlot();
		StorageBox->AddChildToWrapBox(StorageSlotWidget);
		StorageSlotWidgets.Add(StorageSlotWidget);
	}
}

void UStorageWidget::OnCloseButtonClicked()
{
	StorageRef->CloseStorage();
}

void UStorageWidget::OnSlotClicked(int Index)
{
	int AmountToRemove = StorageRef->Slots[Index].Amount;
	int RestAmount = PlayerInventory->AddItem(StorageRef->Slots[Index].ItemClass,AmountToRemove);
	if(RestAmount < AmountToRemove)
	{
		StorageRef->RemoveItemAtIndex(Index,AmountToRemove-RestAmount);
	}
	StorageSlotWidgets[Index]->Base->SetBrushColor(FLinearColor(1,1,1,0));
}

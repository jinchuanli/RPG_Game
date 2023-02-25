// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/InventoryWidget.h"

#include "Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"

void UInventoryWidget::GenerateSlotWidgets()
{
	InventoryBox->ClearChildren();
	InventorySlotWidgets.Empty();
	for(int i = 0; i < InventoryRef->Slots.Num();i++)
	{
		UInventorySlotWidget* InventorySlotWidget = CreateWidget<UInventorySlotWidget>(GetWorld(),
			LoadClass<UInventorySlotWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_InventorySlot.WBP_InventorySlot_C'")));
		InventorySlotWidget->InventoryRef = InventoryRef;
		InventorySlotWidget->SlotIndex = i;
		InventorySlotWidget->UpdateSlot();
		InventoryBox->AddChildToWrapBox(InventorySlotWidget);
		InventorySlotWidgets.Add(InventorySlotWidget);
	}
	UE_LOG(LogTemp,Warning,TEXT("多线程任务执行"));
}

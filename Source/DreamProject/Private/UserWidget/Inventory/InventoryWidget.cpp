// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/InventoryWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/MainUserWidget.h"
#include "Other/StaticLibrary.h"
#include "UserWidget/Inventory/InventoryDragDropOperation.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UInventoryWidget::OnCloseButtonClicked);
	SortButton->OnClicked.AddDynamic(this,&UInventoryWidget::OnSortButtonClicked);
	for(ESortType Type : UStaticLibrary::EnumGetList<ESortType>("ESortType"))
	{
		SortCategoryBox->AddOption(UStaticLibrary::GetEnumValueAsString<ESortType>("ESortType",Type));
	}
	SortCategoryBox->SetSelectedOption(SortCategoryBox->GetOptionAtIndex(0));
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);  //重写一定要先调用父类的东西
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))  //左键按下
	{
		DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}

void UInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(InventoryDragDropOperation);
	TempOp->DefaultDragVisual = this;
	TempOp->Pivot = EDragPivot::MouseDown;
	OutOperation = TempOp;

	if(Cast<UInventoryDragDropOperation>(OutOperation))
	{
		Cast<UInventoryDragDropOperation>(OutOperation)->WidgetToDrag = this;
		Cast<UInventoryDragDropOperation>(OutOperation)->MouseOffest = DragOffset;
	}
	this->RemoveFromParent();
}

void UInventoryWidget::OnSortButtonClicked()
{
	// if(bReversed)
	// {
	// 	InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())),true);
	// 	bReversed = false;
	// }
	// else
	// {
	// 	InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())),false);
	// 	bReversed = true;
	// }
	InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())),bReversed);
	bReversed = !bReversed;
}

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
}

void UInventoryWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	InventoryRef->PlayerCharacter->PC->bEnableClickEvents = false;
	InventoryRef->PlayerCharacter->PC->bShowMouseCursor = false;
	InventoryRef->PlayerCharacter->PC->SetInputMode(InputMode);
	InventoryRef->PlayerCharacter->MainUserWidget->bInventoryVisibling = false;
}

void UInventoryWidget::UpdateWeight(float CurrentWeight, float TotalWeight)
{
	CurrentWeightText->SetText(FText::AsNumber(CurrentWeight));
	TotalWeightText->SetText(FText::AsNumber(TotalWeight));

	if(CurrentWeight > TotalWeight)
	{
		CurrentWeightText->SetColorAndOpacity(FLinearColor::Red);
		TotalWeightText->SetColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		CurrentWeightText->SetColorAndOpacity(FLinearColor::White);
		TotalWeightText->SetColorAndOpacity(FLinearColor::White);
	}
}

void UInventoryWidget::UpdateCoinText(int Coin)
{
	CoinText->SetText(FText::AsNumber(Coin));
}

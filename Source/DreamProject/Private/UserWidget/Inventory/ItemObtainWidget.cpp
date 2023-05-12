// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemObtainWidget.h"
#include "InventorySystem/BaseItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemObtainWidget::Init(TSubclassOf<class ABaseItem> ItemClass,int Amount,class UMainUserWidget* FMainUserWidget)
{
	ABaseItem* Item = ItemClass->GetDefaultObject<ABaseItem>();
	NameText->SetText(Item->ItemInfo.Name);
	AmountText->SetText(FText::AsNumber(Amount));
	IconImage->SetBrushFromTexture(Item->ItemInfo.Icon);
	FLinearColor TempColor;
	switch (Item->ItemInfo.Category)
	{
	case EItemCategories::Consumle:
		TempColor = FLinearColor::White;
		break;
	case EItemCategories::Equipment:
		TempColor = FLinearColor(1,0.03,0,1);
		break;
	case EItemCategories::QuestItem:
		TempColor = FLinearColor(1,0.9,0,1);
		break;
	case EItemCategories::ReadAble:
		TempColor = FLinearColor(1,0.66,1,1);
		break;
	case EItemCategories::Material:
		TempColor = FLinearColor::Green;
		break;
	case EItemCategories::Bow:
		TempColor = FLinearColor(1,0.03,0,1);
		break;
	default:
		break;
	}
	NameText->SetColorAndOpacity(TempColor);
	XText->SetColorAndOpacity(TempColor);
	AmountText->SetColorAndOpacity(TempColor);
	ObtainText->SetColorAndOpacity(TempColor);
	this->MainUserWidget = FMainUserWidget;
}

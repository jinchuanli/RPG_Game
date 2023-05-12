// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemDetailWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Other/StaticLibrary.h"

void UItemDetailWidget::Update()
{
	NameText->SetText(ItemInfo.Name);
	AmountText->SetText(FText::AsNumber(Amount));
	CategoryText->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<EItemCategories>("EItemCategories",ItemInfo.Category)));
	DescriptionText->SetText(ItemInfo.Description);
	IconImage->SetBrushFromTexture(ItemInfo.Icon);
	TotalWeightText->SetText(FText::AsNumber(Amount*ItemInfo.Weight));
	SingleWeightText->SetText(FText::AsNumber(ItemInfo.Weight));
}

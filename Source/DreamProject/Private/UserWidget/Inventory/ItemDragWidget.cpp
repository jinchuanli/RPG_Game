// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemDragWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemDragWidget::Init(int Amount,UTexture2D* Icon)
{
	AmountText->SetText(FText::AsNumber(Amount));
	IconImage->SetBrushFromTexture(Icon);
}

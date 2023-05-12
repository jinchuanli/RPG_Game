// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ReadableItemWidget.h"

#include "Components/Button.h"
#include "InventorySystem/ItemMap.h"

void UReadableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UReadableItemWidget::OnCloseButtonClicked);
}

void UReadableItemWidget::OnCloseButtonClicked()
{
	ItemMap->OnReadOver();
}

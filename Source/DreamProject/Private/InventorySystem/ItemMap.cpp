// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemMap.h"
#include "Character/BasePlayerController.h"
#include "Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ReadableItemWidget.h"

void AItemMap::OnUsed()
{
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(false);
	Map = CreateWidget<UReadableItemWidget>(GetWorld(),
		LoadClass<UReadableItemWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemReadable.WBP_ItemReadable_C'")));
	Map->ItemMap = this;
	InventoryRef->PlayerCharacter->MainUserWidget->ReadableContainer->AddChild(Map);
	//禁止输入事件
	FInputModeUIOnly InputMode;
	InventoryRef->PlayerCharacter->PC->SetInputMode(InputMode);
}

void AItemMap::OnReadOver()
{
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(true);
	if(Map)
		Map->RemoveFromParent();
	FInputModeGameOnly InputMode;
	InventoryRef->PlayerCharacter->PC->SetInputMode(InputMode);
}

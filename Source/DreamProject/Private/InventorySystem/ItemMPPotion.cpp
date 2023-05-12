// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemMPPotion.h"

#include "Character/RBaseCharacter.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"

void AItemMPPotion::OnUsed()
{
	if(InventoryRef->PlayerCharacter->GetCurrentMp() < InventoryRef->PlayerCharacter->GetTotalMp())
	{
		InventoryRef->PlayerCharacter->ChangeCurrentMp(AddMp);
		InventoryRef->PlayerCharacter->MainUserWidget->SetMPPProgressBar(InventoryRef->PlayerCharacter->GetCurrentMp()/InventoryRef->PlayerCharacter->GetTotalMp());
		InventoryRef->RemoveItemAtIndex(Index,1);
		UE_LOG(LogTemp, Warning, TEXT("蓝屏使用成功"));
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("蓝屏使用失败"));
		Destroy();
	}
}

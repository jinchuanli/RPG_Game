// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemHealthPotion.h"

#include "Character/RBaseCharacter.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"

void AItemHealthPotion::OnUsed()
{
	if(InventoryRef->PlayerCharacter->GetCurrentHp() < InventoryRef->PlayerCharacter->GetTotalHp())
	{
		InventoryRef->PlayerCharacter->ChangeCurrentHp(AddHp);
		InventoryRef->PlayerCharacter->MainUserWidget->SetHPPProgressBar(InventoryRef->PlayerCharacter->GetCurrentHp()/InventoryRef->PlayerCharacter->GetTotalHp());
		InventoryRef->RemoveItemAtIndex(Index,1);
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("血瓶使用成功"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("血瓶使用失败"));
		Destroy();
	}
}

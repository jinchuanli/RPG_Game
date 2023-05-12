// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemCoin.h"

#include "Character/RBaseCharacter.h"
#include "UserWidget/MainUserWidget.h"

void AItemCoin::OnIteractWith(ARBaseCharacter* Character)
{
	Character->IncreaseCoin(Amount);
	Character->MainUserWidget->AddItemToObtainedQueue(this->GetClass(),Amount);
	Destroy();
}

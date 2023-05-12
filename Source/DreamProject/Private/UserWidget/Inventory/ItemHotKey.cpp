// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemHotKey.h"
#include "Components/TextBlock.h"

void UItemHotKey::Init(FKey FKey, AInventory* FInventory)
{
	this->Inventory = FInventory;
	this->Key = FKey;
	KeyText->SetText(Key.GetDisplayName());
}

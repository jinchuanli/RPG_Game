// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Arrow/Bow.h"

#include "Character/RBaseCharacter.h"
#include "InventorySystem/Inventory.h"

ABow::ABow()
{
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
}

void ABow::OnUsed()
{
	if(!InventoryRef->PlayerCharacter->EquipItem_Weapon(this))
	{
		Destroy();
	}
}

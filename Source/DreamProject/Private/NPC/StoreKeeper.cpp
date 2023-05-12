// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/StoreKeeper.h"

#include "Character/RBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"

void AStoreKeeper::OnIteractWith(ARBaseCharacter* Character)
{
	if(StorageRef  == nullptr)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if(StorageClass)
			StorageRef = GetWorld()->SpawnActor<AStorage>(StorageClass,Params);
	}

	Character->MainUserWidget->StorageWidget->GenerateSlotWidgets(StorageRef);
	StorageRef->OpenStorage();
}

// void AStoreKeeper::OnEnterPlayerRadius(ARBaseCharacter* Character)
// {
// 	//Super::OnEnterPlayerRadius(Character);
// }

void AStoreKeeper::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	Super::OnLeavePlayerRadius(Character);
	if(StorageRef)
	{
		StorageRef->CloseStorage();
	}
	
}

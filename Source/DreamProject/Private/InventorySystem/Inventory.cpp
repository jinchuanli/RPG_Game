// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory.h"

#include "Character/RBaseCharacter.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	Slots.SetNum(AmountOfSlots);
	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());
}

bool AInventory::bIsSlotEmpty(int Index)
{
	//为空返回t，不为空返回f
	// return !Slots.IsValidIndex(Index);
	if(Slots[Index].ItemClass)
	{
		return false;
	}
	else
	{
		return true;
	}
}

ABaseItem* AInventory::GetItemAtIndex(int Index)
{
	if(!bIsSlotEmpty(Index))
	{
		return Slots[Index].ItemClass->GetDefaultObject<ABaseItem>();
	}
	else
	{
		return nullptr;	
	}
}

void AInventory::UpdateSlotByIndex(int Index)
{
	PlayerCharacter->MainUserWidget->InventoryWidget->InventorySlotWidgets[Index]->UpdateSlot();
}

int AInventory::SearchEmptySlot()
{
	for(int i = 0; i < Slots.Num();i++)
	{
		if(!Slots[i].ItemClass)
		{
			return i;
		}
	}
	return Slots.Num();
}

int AInventory::SearchFreeStack(TSubclassOf<class ABaseItem> Item)
{
	for(int i = 0; i < Slots.Num();i++)
	{
		if(Slots[i].ItemClass && Slots[i].ItemClass == Item && Slots[i].Amount < MaxStackSize)
		{
			return i;
		}
	}
	return Slots.Num();
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AInventory::AddItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	int FoundIndex = 0;
	//先根据物品本生属性来判断
	if(Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked)//该物品可以被堆积，此时可以先寻找一个插槽上空着的堆
	{
		if(SearchFreeStack(Item) == Slots.Num())  //没有不满的堆
		{
			if(SearchEmptySlot() != Slots.Num())//找到了完全空的插槽
			{
				FoundIndex = SearchEmptySlot();
				if(Amount > MaxStackSize)  //多余了，存不下
				{
					Slots[FoundIndex] = FInventorySlot{Item,MaxStackSize};
					UpdateSlotByIndex(FoundIndex);
					return AddItem(Item,Amount - MaxStackSize);  //分批次继续存储
				}
				else  //没有多余，直接存储
				{
					Slots[FoundIndex] = FInventorySlot{Item,Amount};
					UpdateSlotByIndex(FoundIndex);
					return 0;
				}
			}
			else   
			{
				return Amount;
			}
		}
		else //找到不满的堆
		{
			FoundIndex = SearchFreeStack(Item);
			if ((Slots[FoundIndex].Amount + Amount) > MaxStackSize) //插槽当前已经存放的数量+要存放的数量是否大于插槽最大能够存放的数量
			{
				int TempAmount = (Slots[FoundIndex].Amount + Amount) - MaxStackSize;
				Slots[FoundIndex] = FInventorySlot{Item,MaxStackSize};
				UpdateSlotByIndex(FoundIndex);
				return AddItem(Item,TempAmount);
			}
			else
			{
				Slots[FoundIndex] = FInventorySlot{Item,(Slots[FoundIndex].Amount + Amount)};
				UpdateSlotByIndex(FoundIndex);
				return 0;
			}
		}
	}
	else
	{
		//不能被堆积，只能找空插槽，一个插槽存放一个物品
		if(SearchEmptySlot() != Slots.Num())  //找到了空插槽
		{
			FoundIndex = SearchEmptySlot();
			Slots[FoundIndex] = FInventorySlot{Item,1};
			UpdateSlotByIndex(FoundIndex);
			if(Amount > 1)
			{
				//是否一种物品都已经存放完毕了，10个物品不是一下子存放十个，而是存放十次
				return AddItem(Item,Amount - 1);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return Amount;
		}
	}
}


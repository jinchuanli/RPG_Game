// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Storage.h"
#include "InventorySystem/Inventory.h"
#include "Character/RBaseCharacter.h"
#include "InventorySystem/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/StorageSave.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"

// Sets default values
AStorage::AStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStorage::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
	StorageWidget = PlayerCharacter->MainUserWidget->StorageWidget;

	if(UGameplayStatics::DoesSaveGameExist(SavedSlotName,0))
		LoadStorage();
	
	Slots.SetNum(AmountOfSlots);
}

int AStorage::SearchEmptySlot()
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

int AStorage::SearchFreeStack(TSubclassOf<ABaseItem> Item)
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

int AStorage::AddItem(TSubclassOf<ABaseItem> Item, int Amount)
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

bool AStorage::bIsSlotEmpty(int Index)
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

ABaseItem* AStorage::GetItemAtIndex(int Index)
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

void AStorage::UpdateSlotByIndex(int Index)
{
	StorageWidget->StorageSlotWidgets[Index]->UpdateSlot();
}

int AStorage::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amount;
}

bool AStorage::SwapSlot(int Index1, int Index2)
{
	if(Index1 >= Slots.Num() || Index2 >= Slots.Num())
	{
		return false;
	}
	else
	{
		FInventorySlot TempSlot = Slots[Index1];
		Slots[Index1] = Slots[Index2];
		Slots[Index2] = TempSlot;
		UpdateSlotByIndex(Index1);
		UpdateSlotByIndex(Index2);
		return true;
	}
}

bool AStorage::AddToIndex(int FromIndex, int ToIndex)
{
	if(Slots[FromIndex].ItemClass == Slots[ToIndex].ItemClass && GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked
		&& GetAmountAtIndex(ToIndex) < MaxStackSize)
	{
		if((GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex)) < MaxStackSize)
		{
			Slots[ToIndex] = FInventorySlot{Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex)};
			Slots[FromIndex] = FInventorySlot{nullptr,0};
		}
		else
		{
			Slots[FromIndex] = FInventorySlot{Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex)+GetAmountAtIndex(ToIndex) - MaxStackSize};
			Slots[ToIndex] = FInventorySlot{Slots[FromIndex].ItemClass,MaxStackSize};
		}
		UpdateSlotByIndex(FromIndex);
		UpdateSlotByIndex(ToIndex);
		return true;
	}
	else
	{
		return false;
	}
}

bool AStorage::RemoveItemAtIndex(int Index, int Amount)
{
	if(!bIsSlotEmpty(Index) && Amount > 0)
	{
		if(Amount >= GetAmountAtIndex(Index))
		{
			Slots[Index] = FInventorySlot{nullptr,0};
			UpdateSlotByIndex(Index);
			return true;
		}
		else
		{
			Slots[Index] = FInventorySlot{Slots[Index].ItemClass,GetAmountAtIndex(Index) - Amount};
			UpdateSlotByIndex(Index);
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool AStorage::AddItemToIndex(int Index, TSubclassOf<ABaseItem> Item, int Amount)
{
	if(bIsSlotEmpty(Index) && Amount < MaxStackSize)
	{
		Slots[Index] = FInventorySlot{Item,Amount};
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

bool AStorage::IncreaseAmountAtIndex(int Index, int Amount)
{
	if(!bIsSlotEmpty(Index) && (GetAmountAtIndex(Index) + Amount) <= MaxStackSize)
	{
		Slots[Index].Amount += Amount;
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

void AStorage::OpenStorage()
{
	bStorageOpen = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerCharacter->PC->SetInputMode(InputMode);
	PlayerCharacter->PC->bEnableClickEvents = true;
	PlayerCharacter->PC->bShowMouseCursor = true;
	StorageWidget->SetVisibility(ESlateVisibility::Visible);
}

void AStorage::CloseStorage()
{
	bStorageOpen = false;
	FInputModeGameOnly InputMode;
	PlayerCharacter->PC->bEnableClickEvents = false;
	PlayerCharacter->PC->bShowMouseCursor = false;
	PlayerCharacter->PC->SetInputMode(InputMode);
	StorageWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool AStorage::MoveFromInventoryToStorageByIndex(AInventory* Inventory, int InventoryIndex, int StorageIndex)
{
	int AmountToAdd;
	if(bIsSlotEmpty(StorageIndex))//判断仓库的插槽是否为空
	{
		AmountToAdd = Inventory->GetAmountAtIndex(InventoryIndex);
		if(AddItemToIndex(StorageIndex,Inventory->Slots[InventoryIndex].ItemClass,AmountToAdd))
		{
			Inventory->RemoveItemAtIndex(InventoryIndex,AmountToAdd);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(Slots[StorageIndex].ItemClass == Inventory->Slots[InventoryIndex].ItemClass && GetItemAtIndex(StorageIndex)->ItemInfo.bCanBeStacked && GetAmountAtIndex(StorageIndex) < MaxStackSize)
		{
			AmountToAdd = (MaxStackSize - GetAmountAtIndex(StorageIndex)) < Inventory->GetAmountAtIndex(InventoryIndex)
			? (MaxStackSize - GetAmountAtIndex(StorageIndex))
			: Inventory->GetAmountAtIndex(InventoryIndex);
			if(IncreaseAmountAtIndex(StorageIndex,AmountToAdd))
			{
				Inventory->RemoveItemAtIndex(InventoryIndex,AmountToAdd);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

void AStorage::SaveStorage()
{
	if(!StorageSaveInstance)
		StorageSaveInstance = Cast<UStorageSave>(UGameplayStatics::CreateSaveGameObject(UStorageSave::StaticClass()));

	StorageSaveInstance->SavedSlots = Slots;

	UGameplayStatics::SaveGameToSlot(StorageSaveInstance,SavedSlotName,0);
}

void AStorage::LoadStorage()
{
	if(!StorageSaveInstance)
	{
		StorageSaveInstance =Cast<UStorageSave>(UGameplayStatics::LoadGameFromSlot(SavedSlotName,0));

		Slots = StorageSaveInstance->SavedSlots;
	}
}

// Called every frame
void AStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory.h"

#include "Character/RBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "InventorySystem/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/InventorySave.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "InventorySystem/Storage.h"
#include "InventorySystem/Arrow/Bow.h"
#include "UserWidget/Inventory/ItemHotKey.h"
#include "UserWidget/Inventory/ItemHotKeyWidget.h"

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

	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());

	if(UGameplayStatics::DoesSaveGameExist(SaveSlotName,0))
	{
		LoadInventory();
		if(CurrentWeight > TotalWeight)
		{
			bOverLoad = true;
			PlayerCharacter->OnOverLoaded();
		}
	}
	
	Slots.SetNum(AmountOfSlots);
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
	UpdateHotKeyByIndex(Index);
}

int AInventory::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amount;
}

bool AInventory::RemoveItemAtIndex(int Index, int Amount)
{
	if(!bIsSlotEmpty(Index) && Amount > 0)
	{
		if(Amount >= GetAmountAtIndex(Index))
		{
			RemoveWeightForItem(Slots[Index].ItemClass,GetAmountAtIndex(Index));
			Slots[Index] = FInventorySlot{nullptr,0};
			UpdateSlotByIndex(Index);
			UpdateCraftingMenu();
			return true;
		}
		else
		{
			RemoveWeightForItem(Slots[Index].ItemClass,Amount);
			Slots[Index] = FInventorySlot{Slots[Index].ItemClass,GetAmountAtIndex(Index) - Amount};
			UpdateSlotByIndex(Index);
			UpdateCraftingMenu();
			return true;
		}
	}
	else
	{
		return false;
	}
}

// bool  AInventory::SetColorEquipment(int Index)
// {
// 	ABaseItem* temp = Cast<ABaseItem>(Slots[Index].ItemClass);
// 	if(temp)
// 	{
// 		if(temp->ItemInfo.Category == EItemCategories::Equipment)
// 		{
// 			UpdateSlotByIndex(Index);
// 			UE_LOG(LogTemp, Warning, TEXT("装备UI更新"));
// 			return true;
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("装备UI不更新"));
// 			return false;
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("装备UI不不不不更新"));
// 		return false;
// 	}
// }

bool AInventory::SwapSlot(int Index1, int Index2)
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
		HandleSwapForHotKeys(Index1,Index2);
		return true;
	}
}

bool AInventory::SplitStacke(int Index, int Amount)
{
	if(!bIsSlotEmpty(Index))
	{
		if(GetAmountAtIndex(Index) > Amount && GetItemAtIndex(Index)->ItemInfo.bCanBeStacked)
		{
			int FoundIndex = SearchEmptySlot();
			if(FoundIndex != Slots.Num())
			{
				Slots[Index] = FInventorySlot{Slots[Index].ItemClass,GetAmountAtIndex(Index) - Amount};
				Slots[FoundIndex] = FInventorySlot{Slots[Index].ItemClass,Amount};
				UpdateSlotByIndex(Index);
				UpdateSlotByIndex(FoundIndex);
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
	else
	{
		return false;
	}
}

void AInventory::UseItemAtIndex(int Index)
{
	if(Slots[Index].ItemClass)
	{
	
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(Slots[Index].ItemClass,Params);
		Item->InventoryRef = this;
		Item->Index = Index;
		if(Item->ItemInfo.Category == EItemCategories::Equipment)
		{
			Item->BoxCollisonComp->SetGenerateOverlapEvents(false);
		}
		if(Item->ItemInfo.Category == EItemCategories::Bow)
		{
			Item->BoxCollisonComp->SetGenerateOverlapEvents(false);
			//PlayerCharacter->SpawnBow();
		}
		Item->OnUsed();

	}
}

void AInventory::UpdateActionMenuPosition(UInventorySlotWidget* Slot)
{
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->Update(Slot->SlotIndex);
	int Y = Slot->SlotIndex / 7;
	int X = Slot->SlotIndex % 7;
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetRenderTranslation(FVector2D(X*93,Y*93));
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Visible);
}

bool AInventory::AddToIndex(int FromIndex, int ToIndex)
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

bool AInventory::SplitStackToIndex(int FromIndex, int ToIndex, int Amount)
{
	if(!bIsSlotEmpty(FromIndex) && bIsSlotEmpty(ToIndex) && GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked && GetAmountAtIndex(FromIndex) > 2
		&& GetAmountAtIndex(FromIndex) > Amount)
	{
		Slots[FromIndex] = FInventorySlot{Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex) - Amount};
		Slots[ToIndex] = FInventorySlot{Slots[FromIndex].ItemClass,Amount};
		UpdateSlotByIndex(FromIndex);
		UpdateSlotByIndex(ToIndex);
		return true;
	}
	else
	{
		return false;
	}
}

void AInventory::SortInventory(ESortType Type, bool BReversed)
{
	if(FilterEmptySlot().Num() > 1)
	{
		Slots = BubbleSortArray(FilterEmptySlot(),Type,BReversed);
		Slots.SetNum(AmountOfSlots);
		for(int i = 0;i < Slots.Num();i++)
		{
			UpdateSlotByIndex(i);
		}
	}
	else
	{
		Slots = FilterEmptySlot();
		Slots.SetNum(AmountOfSlots);
		for(int i = 0;i < Slots.Num();i++)
		{
			UpdateSlotByIndex(i);
		}
	}
}

TArray<FInventorySlot> AInventory::FilterEmptySlot()
{
	TArray<FInventorySlot> TempSlots;
	for(FInventorySlot Slot : Slots)
	{
		if(Slot.ItemClass)
		{
			TempSlots.Add(Slot);
		}
	}
	return TempSlots;
}

TArray<FInventorySlot> AInventory::BubbleSortArray(TArray<FInventorySlot> InArray, ESortType Type, bool bReversed)
{
	TArray<FInventorySlot> TempSlots = InArray;
	for(int End = TempSlots.Num() - 1; End > 0;End--)
	{
		for(int i = 0; i < End;i++)
		{
			switch (Type)
			{
			case ESortType::Category:
				if(!bReversed)
				{
					if(TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category > TempSlots[i+1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				else
				{
					if(TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category < TempSlots[i+1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				break;
			case ESortType::Amount:
				if(!bReversed)
				{
					if(TempSlots[i].Amount > TempSlots[i + 1].Amount)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				else
				{
					if(TempSlots[i].Amount < TempSlots[i + 1].Amount)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				break;
			case ESortType::Name:
				if(!bReversed)
				{
					if(TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString().Compare(TempSlots[i+1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString()) > 0)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				else
				{
					if(TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString().Compare(TempSlots[i+1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString()) < 0)
					{
						FInventorySlot TempSlot;
						TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i+1];
						TempSlots[i+1] = TempSlot;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return TempSlots;
}

void AInventory::UpdateWeight()
{
	PlayerCharacter->MainUserWidget->InventoryWidget->UpdateWeight(CurrentWeight,TotalWeight);
}

void AInventory::AddWeight(float Weight)
{
	CurrentWeight += Weight;
	UpdateWeight();
	if(!bOverLoad && CurrentWeight > TotalWeight)
	{
		bOverLoad = true;
		PlayerCharacter->OnOverLoaded();
	}
}

void AInventory::RemoveWeight(float Weight)
{
	CurrentWeight -= Weight;
	UpdateWeight();
	if(bOverLoad && CurrentWeight <= TotalWeight)
	{
		bOverLoad = false;
		PlayerCharacter->OnOverLoadEnd();
	}
}

void AInventory::AddWeightForItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	AddWeight(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weight*Amount);
}

void AInventory::RemoveWeightForItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	RemoveWeight(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weight*Amount);
}

int AInventory::GetTotalAmountOfItem(TSubclassOf<ABaseItem> Item, TArray<int>& ItemIndices)
{
	int TempAmount = 0;
	for(int i = 0;i < Slots.Num();i++)
	{
		if(Slots[i].ItemClass)
		{
			if(Slots[i].ItemClass == Item)
			{
				TempAmount += Slots[i].Amount;
				ItemIndices.Add(i);//把索引传入数组中
			}
		}
	}
	return TempAmount;
}

bool AInventory::RemoveItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	TArray<int> ItemIndices;
	if(GetTotalAmountOfItem(Item,ItemIndices) >= Amount)
	{
		for(int Index : ItemIndices)
		{
			if(GetAmountAtIndex(Index) >= Amount)
			{
				RemoveItemAtIndex(Index,Amount);
				break;
			}
			else
			{
				Amount -= GetAmountAtIndex(Index);
				RemoveItemAtIndex(Index,GetAmountAtIndex(Index));
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void AInventory::UpdateCraftingMenu()
{
	if(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem)
		PlayerCharacter->MainUserWidget->CraftingMenu->UpdateDetailWindow(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem);
}

void AInventory::SaveInventory()
{
	//创建实例
	if(!InventorySaveinstance)
		InventorySaveinstance = Cast<UInventorySave>(UGameplayStatics::CreateSaveGameObject(UInventorySave::StaticClass()));
	
	//赋值
	InventorySaveinstance->SavedWeight = CurrentWeight;
	InventorySaveinstance->SavedSlots = Slots;
	InventorySaveinstance->SavedPickups = LootedPickups;

	//保存实例
	UGameplayStatics::SaveGameToSlot(InventorySaveinstance,SaveSlotName,0);
}

bool AInventory::BContainsID(int InID, int& Rest)
{
	for(FSavedPickup PickUp : LootedPickups)
	{
		if(PickUp.ID == InID)
		{
			Rest = PickUp.RestAmount;
			return true;
		}
	}
	return false;
}

void AInventory::LoadPickups()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ABaseItem::StaticClass(),FoundActors);

	for(AActor* Actor : FoundActors)
	{
		ABaseItem* Item = Cast<ABaseItem>(Actor);
		{
			if(Item)
			{
				if(Item->ID != -1)
				{
					int Rest;
					if(BContainsID(Item->ID,Rest))
					{
						if(Rest > 0)
						{
							Item->Amount = Rest;
						}
						else
						{
							Item->Destroy();
						}
					}
				}
			}
		}
	}
}

void AInventory::LoadInventory()
{
	if(!InventorySaveinstance)
		InventorySaveinstance = Cast<UInventorySave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,0));

	CurrentWeight = InventorySaveinstance->SavedWeight;
	Slots = InventorySaveinstance->SavedSlots;
	LootedPickups = InventorySaveinstance->SavedPickups;

	LoadPickups();
}

bool AInventory::AddItemToIndex(int Index, TSubclassOf<ABaseItem> Item, int Amount)
{
	if(bIsSlotEmpty(Index) && Amount < MaxStackSize)
	{
		Slots[Index] = FInventorySlot{Item,Amount};
		AddWeightForItem(Item,Amount);
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

bool AInventory::IncreaseAmountAtIndex(int Index, int Amount)
{
	if(!bIsSlotEmpty(Index) && (GetAmountAtIndex(Index) + Amount) <= MaxStackSize)
	{
		Slots[Index].Amount += Amount;
		AddWeightForItem(Slots[Index].ItemClass,Slots[Index].Amount);
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

bool AInventory::MoveFromStorageToInventoryByIndex(AStorage* Storage, int StorageIndex, int InventoryIndex)
{
	int AmountToAdd;
	if(bIsSlotEmpty(InventoryIndex))//判断仓库的插槽是否为空
		{
		AmountToAdd = Storage->GetAmountAtIndex(StorageIndex);
		if(AddItemToIndex(InventoryIndex,Storage->Slots[StorageIndex].ItemClass,AmountToAdd))
		{
			Storage->RemoveItemAtIndex(StorageIndex,AmountToAdd);
			return true;
		}
		else
		{
			return false;
		}
		}
	else
	{
		if(Slots[InventoryIndex].ItemClass == Storage->Slots[StorageIndex].ItemClass && GetItemAtIndex(InventoryIndex)->ItemInfo.bCanBeStacked && GetAmountAtIndex(InventoryIndex) < MaxStackSize)
		{
			AmountToAdd = (MaxStackSize - GetAmountAtIndex(InventoryIndex)) < Storage->GetAmountAtIndex(StorageIndex)
			? (MaxStackSize - GetAmountAtIndex(InventoryIndex))
			: Storage->GetAmountAtIndex(StorageIndex);
			if(IncreaseAmountAtIndex(InventoryIndex,AmountToAdd))
			{
				Storage->RemoveItemAtIndex(StorageIndex,AmountToAdd);
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

void AInventory::UpdateHotKeyByIndex(int Index)
{
	for(UItemHotKey* HotKey : PlayerCharacter->MainUserWidget->ItemHotKeyWidgets)
	{
		if(!HotKey->ItemHotKeySlot->bEmpty && HotKey->ItemHotKeySlot->InventoryIndex == Index)
		{
			HotKey->ItemHotKeySlot->Update();
		}
	}
}

void AInventory::HandleItemHotKeyPress(FKey Key)
{
	for(UItemHotKey* HotKey :  PlayerCharacter->MainUserWidget->ItemHotKeyWidgets)
	{
		if(HotKey->Key == Key && !HotKey->ItemHotKeySlot->bEmpty)
		{
			UseItemAtIndex(HotKey->ItemHotKeySlot->InventoryIndex);
			break;
		}
	}
}

void AInventory::HandleSwapForHotKeys(int Index1, int Index2)
{
	for(UItemHotKey* HotKey :  PlayerCharacter->MainUserWidget->ItemHotKeyWidgets)
	{
		if(!HotKey->ItemHotKeySlot->bEmpty)
		{
			if(HotKey->ItemHotKeySlot->InventoryIndex == Index1)
			{
				HotKey->ItemHotKeySlot->InventoryIndex = Index2;
			}
			else if(HotKey->ItemHotKeySlot->InventoryIndex == Index2)
			{
				HotKey->ItemHotKeySlot->InventoryIndex = Index1;
			}
		}
	}
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
					AddWeightForItem(Item,MaxStackSize);
					UpdateCraftingMenu();
					UpdateSlotByIndex(FoundIndex);
					return AddItem(Item,Amount - MaxStackSize);  //分批次继续存储
				}
				else  //没有多余，直接存储
				{
					Slots[FoundIndex] = FInventorySlot{Item,Amount};
					AddWeightForItem(Item,Amount);
					UpdateCraftingMenu();
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
				AddWeightForItem(Item,MaxStackSize);
				UpdateCraftingMenu();
				UpdateSlotByIndex(FoundIndex);
				return AddItem(Item,TempAmount);
			}
			else
			{
				Slots[FoundIndex] = FInventorySlot{Item,(Slots[FoundIndex].Amount + Amount)};
				AddWeightForItem(Item,Amount);
				UpdateCraftingMenu();
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
			AddWeightForItem(Item,1);
			UpdateCraftingMenu();
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


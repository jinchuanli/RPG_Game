// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStruct.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

UCLASS()
class DREAMPROJECT_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();
	class ARBaseCharacter* PlayerCharacter;

	float CurrentWeight;
	UPROPERTY(EditAnywhere,Category=InventoryInfo)
	float TotalWeight = 500;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//插槽的数量
	int AmountOfSlots = 42;
	int MaxStackSize = 99;  // 一个插槽最多存放的数量

	//查找是否有空的插槽
	int SearchEmptySlot();
//寻找不满的堆
	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);
public:
	TArray<FInventorySlot> Slots; //用来存储物品信息
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//添加物品的函数,返回添加后剩下的数量
	int AddItem(TSubclassOf<class ABaseItem> Item,int Amount);

	bool bIsSlotEmpty(int Index);

	class ABaseItem* GetItemAtIndex(int Index);

	void UpdateSlotByIndex(int Index);

	int GetAmountAtIndex(int Index);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int Index,int Amount);

	//bool SetColorEquipment(int Index);
	
	UFUNCTION(BlueprintCallable)
	bool SwapSlot(int Index1,int Index2);//交换物品
	UFUNCTION(BlueprintCallable)
	bool SplitStacke(int Index,int Amount);
	UFUNCTION(BlueprintCallable)
	void UseItemAtIndex(int Index);

	void UpdateActionMenuPosition(class UInventorySlotWidget* Slot);

	bool AddToIndex(int FromIndex,int ToIndex);

	bool SplitStackToIndex(int FromIndex,int ToIndex,int Amount);

	//控制背包排序的逻辑
	void SortInventory(ESortType Type,bool BReversed);
	//过滤空的格子
	TArray<FInventorySlot> FilterEmptySlot();

	TArray<FInventorySlot> BubbleSortArray(TArray<FInventorySlot> InArray,ESortType Type,bool bReversed);

	bool bOverLoad;
	void UpdateWeight();
	void AddWeight(float Weight);
	void RemoveWeight(float Weight);

	void AddWeightForItem(TSubclassOf<ABaseItem> Item,int Amount);
	void RemoveWeightForItem(TSubclassOf<ABaseItem> Item,int Amount);

	//统计背包中物品的数量
	int GetTotalAmountOfItem(TSubclassOf<ABaseItem> Item,TArray<int> &ItemIndices);
	bool RemoveItem(TSubclassOf<ABaseItem> Item,int Amount);


	//可以合成的物品
	UPROPERTY(EditAnywhere,Category=Item)
	TArray<TSubclassOf<ABaseItem>> CraftableItems;

	void UpdateCraftingMenu();

	/////////////////////////////////////////////////////////////////////////////////////save
	TArray<FSavedPickup> LootedPickups;
	class UInventorySave* InventorySaveinstance;
	FString SaveSlotName = "InventorySave";

	void SaveInventory();

	bool BContainsID(int InID,int &Rest);
	void LoadPickups();

	void LoadInventory();

	////////////////////////////////////////////////////////////////////////////////warehouse
	bool AddItemToIndex(int Index,TSubclassOf<ABaseItem> Item,int Amount);
	bool IncreaseAmountAtIndex(int Index,int Amount);

	bool MoveFromStorageToInventoryByIndex(class AStorage* Storage,int StorageIndex,int InventoryIndex);


	///////////////////////////////////////////////////////////////////
	void UpdateHotKeyByIndex(int Index);
	void HandleItemHotKeyPress(FKey Key);

	void HandleSwapForHotKeys(int Index1,int Index2);
	
};

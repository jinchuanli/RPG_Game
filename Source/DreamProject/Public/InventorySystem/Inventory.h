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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//插槽的数量
	int AmountOfSlots = 42;
	int MaxStackSize = 99;  // 一个插槽最多存放的数量
	class ARBaseCharacter* PlayerCharacter;



	//查找是否有空的插槽
	int SearchEmptySlot();
//寻找不满的堆
	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);
public:
	TArray<FInventorySlot> Slots;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//添加物品的函数,返回添加后剩下的数量
	int AddItem(TSubclassOf<class ABaseItem> Item,int Amount);

	bool bIsSlotEmpty(int Index);

	class ABaseItem* GetItemAtIndex(int Index);

	void UpdateSlotByIndex(int Index);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryEnum.h"
#include "UObject/NoExportTypes.h"
#include "InventoryStruct.generated.h"

//插槽结构体
USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	TSubclassOf<class ABaseItem> ItemClass;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	int Amount;
};

USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	FText Name;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	FText Description;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	class UTexture2D* Icon;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	bool bCanBeUsed = true;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	FText UseText;//使用说明
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	bool bCanBeStacked;//物品是否可以堆积
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	EItemCategories Category = EItemCategories::Consumle;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	FString ComparaName;
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	float Weight;

	//合成的物品所需要的信息（物品及数量）
	UPROPERTY(EditAnywhere,Category= ItemInfo)
	TArray<FInventorySlot> Recipe;

	UPROPERTY(EditAnywhere,Category= ItemInfo)
	float Price;
};

USTRUCT()
struct FSavedPickup
{
	GENERATED_BODY()
	UPROPERTY()
	int ID;//场景中所有需要保存的物体的ID，同一个类不同物品ID不同，如果该物体不需要保存ID = -1；重新开始游戏不会生成
	UPROPERTY()
	int RestAmount;
};

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryStruct : public UObject
{
	GENERATED_BODY()
	
};

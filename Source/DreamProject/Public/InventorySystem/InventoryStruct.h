// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryEnum.h"
#include "UObject/NoExportTypes.h"
#include "InventoryStruct.generated.h"

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
};

//插槽结构体
USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()
	TSubclassOf<class ABaseItem> ItemClass;
	int Amount;
};

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryStruct : public UObject
{
	GENERATED_BODY()
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryEnum.generated.h"

UENUM()
enum class EItemCategories
{
	Consumle,//消耗品
	Equipment,//装备
	Bow,
	QuestItem,//任务书
	ReadAble,//可以阅读
	Material
};

UENUM()
enum class ESortType
{
	Category,
	Amount,
	Name,
};
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryEnum : public UObject
{
	GENERATED_BODY()
	
};

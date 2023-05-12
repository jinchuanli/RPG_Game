// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemHotKey.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemHotKey : public UUserWidget
{
	GENERATED_BODY()
	friend class AInventory;

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* KeyText;
	UPROPERTY(meta=(BindWidget))
	class UItemHotKeyWidget* ItemHotKeySlot;
	FKey Key;
	class AInventory* Inventory;


	void Init(FKey Key,class AInventory* Inventory);
};

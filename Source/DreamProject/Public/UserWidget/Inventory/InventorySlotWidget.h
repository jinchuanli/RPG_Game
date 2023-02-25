// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UButton* SlotButton;
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AmountText;

protected:

	int Amount;

	FItemInfo ItemInfo;

public:
	int SlotIndex;
	class AInventory* InventoryRef;
	void UpdateSlot();
};

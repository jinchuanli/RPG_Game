// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	class UInventorySlotWidget* DraggedSlot;  //从那个插槽上拖出来的
	bool bAltDown;
};

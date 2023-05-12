// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	class UUserWidget* WidgetToDrag;
	FVector2D MouseOffest;
};

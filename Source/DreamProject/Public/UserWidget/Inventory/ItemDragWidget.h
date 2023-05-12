// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ItemDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;

public:
	void Init(int Amount,UTexture2D* Icon);
};

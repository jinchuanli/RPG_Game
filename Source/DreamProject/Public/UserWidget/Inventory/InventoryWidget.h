// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* InventoryBox;
	UPROPERTY(meta=(BindWidget))
	class UButton* CloseButton;

public:
	TArray<class UInventorySlotWidget*> InventorySlotWidgets;

	class AInventory* InventoryRef;
	
	//生成插槽的函数
	//UFUNCTION(BlueprintCallable,Category= UI)
	void GenerateSlotWidgets();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemHotKeyWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemHotKeyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UBorder* Bg;
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(meta=(BindWidget))
	class USizeBox* UseState;


	bool bEmpty = true;//快捷键上是否有物品
	int InventoryIndex;//拖过来的物品在背包中的那个插槽上，是插槽的索引
	class AInventory* Inventory;
	FLinearColor DefaultTint = FLinearColor(0.086,0.086,0.09,1);
	FLinearColor DragOverTint = FLinearColor(1,0.478,0,1);
	bool bDraggedOver;

	void Update();
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;
	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	virtual void NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
};

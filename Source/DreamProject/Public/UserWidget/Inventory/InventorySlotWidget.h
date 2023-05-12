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
	UPROPERTY(meta=(BindWidget))
	class UBorder* Base;

	virtual void NativeConstruct() override;
public:

	int Amount;

	FItemInfo ItemInfo;

	// 图片需要跟着DragOperation才能走
	UPROPERTY(EditAnywhere,Category=Drag)
	TSubclassOf<class UDragDropOperation> ItemDragOperation;

	int SlotIndex;
	class AInventory* InventoryRef;
	void UpdateSlot();

	int clickCount;  //点击的次数
	FTimerHandle TimerHandle_CountToZero;
	void SetCountToZero();

	UFUNCTION()
	void OnSlotButtonClicked();

	//重写双击的函数
	//virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	//virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	//拖拽slot物品
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	//检测到在拖拽
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;

	//悬停在图标上的显示
	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	bool bDraggedOver;
	//鼠标离开slot的时候
	virtual void NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	//检测放下
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation );/////////

	/**
	 * @brief //////////////////////////////////////////////////////////////////////////
	 */
	class UItemDetailWidget* ItemDetail;
	
};

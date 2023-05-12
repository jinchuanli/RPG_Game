// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "StorageSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UStorageSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class UStorageWidget;
	UPROPERTY(meta=(BindWidget))
	class UButton* SlotButton;
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(meta=(BindWidget))
	class UBorder* Base;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere,Category=DragDrop)
	TSubclassOf<class UDragDropOperation> StorageSlotDrag;
public:

	int Amount;

	FItemInfo ItemInfo;

	int SlotIndex;
	class AStorage* StorageRef;
	class UStorageWidget* StorageWidget;
	void UpdateSlot();
	
	UFUNCTION()
	void OnSlotButtonClicked();
	
	//拖拽slot物品
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;    //鼠标检测都是以一样的
	//检测到在拖拽
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;  //检测到鼠标的移动

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

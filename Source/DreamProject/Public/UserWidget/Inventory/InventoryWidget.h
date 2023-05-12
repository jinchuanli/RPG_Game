// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	class UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* SortCategoryBox;
	UPROPERTY(meta=(BindWidget))
	class UButton* SortButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentWeightText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalWeightText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CoinText;
	
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditAnywhere,Category=DragDrop)
	TSubclassOf<class UDragDropOperation> InventoryDragDropOperation;   //在男途中进行赋值

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )override;

	bool bReversed = false;
	UFUNCTION()
	void OnSortButtonClicked();
public:
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* InventoryBox;
	
	TArray<class UInventorySlotWidget*> InventorySlotWidgets;

	class AInventory* InventoryRef;
	
	//生成插槽的函数
	//UFUNCTION(BlueprintCallable,Category= UI)
	void GenerateSlotWidgets();

	UFUNCTION()
	void OnCloseButtonClicked();

	FVector2D DragOffset;
	
	UPROPERTY(meta=(BindWidget))
	class UActionMenuWidget* ActionMenu;

	void UpdateWeight(float CurrentWeight,float TotalWeight);

	void UpdateCoinText(int Coin);
};

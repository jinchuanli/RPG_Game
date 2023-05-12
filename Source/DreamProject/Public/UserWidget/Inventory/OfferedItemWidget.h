// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "OfferedItemWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class USlider;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UOfferedItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PriceText;
	UPROPERTY(meta=(BindWidget))
	UButton* BuyButton;
	UPROPERTY(meta=(BindWidget))
	USlider* AmountSlider;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AmountText;

	class AInventory* Inventory;
	TSubclassOf<class ABaseItem> AssignedItem;
	FItemInfo ItemInfo;
	int CurrentAmount = 1;
	UPROPERTY(EditAnywhere,Category=ItemInfo)
	int MaxAmount = 99;

virtual void NativeConstruct() override;

	UFUNCTION()
	void OnAmountSliderValueChanged(float Value);

	UFUNCTION()
	void OnBuyButtonClicked();

	class UShopWidget* ShopWidget;
public:
	void UpdatePrice();

	void Init(TSubclassOf<ABaseItem> Item,class AInventory* FInventory,class UShopWidget* FShopWidget);

	
};

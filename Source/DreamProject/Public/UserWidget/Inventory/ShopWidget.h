// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OfferedItemWidget.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UButton;
class UTextBlock;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class AMerchant;
	friend class USellWidget;
	
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* GridPanel;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CoinText;
	

	class ARBaseCharacter* PlayerCharacter;
	class AMerchant* Merchant;
	TArray<class UOfferedItemWidget*> OfferedItemWidgets;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnCloseButtonClicked();

	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;

public:
	UPROPERTY(meta=(BindWidget))
	class USellWidget* SellWidget;
	
	void UpdateCoin();

	void GenerateItemList();

	void UpdateAllItems();
};

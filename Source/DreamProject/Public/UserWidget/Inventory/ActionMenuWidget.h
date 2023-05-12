// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ActionMenuWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UButton* UseItemButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ThrowAwayButton;
	UPROPERTY(meta=(BindWidget))
	UButton* SplitStacksbutton;
	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* UseText;
	
	int CurrentIndex;
	FItemInfo ItemInfo;
	int Amount;

	virtual void NativeConstruct() override;

public:
	void Update(int Index);

	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnThrowButtonClicked();
	UFUNCTION()
	void OnSplitButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

	class AInventory* InventoryRef;
};

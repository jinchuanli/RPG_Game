// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ItemDetailWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AmountText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DescriptionText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CategoryText;
	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalWeightText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SingleWeightText;

public:
	FItemInfo ItemInfo;
	int Amount;

	void Update();
};

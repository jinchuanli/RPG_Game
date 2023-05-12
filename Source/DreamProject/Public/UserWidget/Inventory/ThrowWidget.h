// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ThrowWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UThrowWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountText;
	UPROPERTY(meta=(BindWidget))
	UButton* MinusButton;
	UPROPERTY(meta=(BindWidget))
	UButton* PlusButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ConfirmButton;
	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;

	int ThrowCount;  //当前要丢弃的数量
	int MaxAmount;  //插槽上的数量
	int CurrentIndex;
	FItemInfo ItemInfo;

	int ClikCount;
	FTimerHandle TimerHandle_CounttoZero;

	FTimerHandle TimerHandle_Increase;
	FTimerHandle TimerHandle_Decrease;

	virtual void NativeConstruct() override;
public:
	class AInventory* InventoryRef;
	void Update(int Index);

	void IncreaseCount();  
	void DecreaseCount();

	UFUNCTION()
	void OnMinusButtonClicked();
	UFUNCTION()
	void OnPlusButtonClicked();

	void SetClickCountToZero();

	UFUNCTION()
	void OnMinusButtonPressed();
	UFUNCTION()
	void OnPlusButtonPressed();
	UFUNCTION()
	void OnMinusButtonReleased();
	UFUNCTION()
	void OnPlusButtonReleased();

	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();
};

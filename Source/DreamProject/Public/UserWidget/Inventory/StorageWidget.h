// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UStorageWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	class UButton* CloseButton;
	
	virtual void NativeConstruct() override;
	

public:
	class AInventory* PlayerInventory;
	
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* StorageBox;
	
	TArray<class UStorageSlotWidget*> StorageSlotWidgets;

	class AStorage* StorageRef;
	
	//生成插槽的函数
	//UFUNCTION(BlueprintCallable,Category= UI)
	void GenerateSlotWidgets(class AStorage* FStorageRef);

	UFUNCTION()
	void OnCloseButtonClicked();
	
	void OnSlotClicked(int Index);
};

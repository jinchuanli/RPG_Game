// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadableItemWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UReadableItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UImage* MapImage;

virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnCloseButtonClicked();
	
public:
	class AItemMap* ItemMap;
};

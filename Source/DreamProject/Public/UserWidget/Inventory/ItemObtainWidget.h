// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemObtainWidget.generated.h"


class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UItemObtainWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AmountText;
	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ObtainText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* XText;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float VisibleDuration = 2.0f;

	UPROPERTY(BlueprintReadOnly)
	class UMainUserWidget* MainUserWidget;
public:
	void Init(TSubclassOf<class ABaseItem> ItemClass,int Amount,class UMainUserWidget* FMainUserWidget);
};

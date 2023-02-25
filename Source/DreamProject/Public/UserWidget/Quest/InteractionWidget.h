// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetNameText(FText Name);
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* InteractionText;
};

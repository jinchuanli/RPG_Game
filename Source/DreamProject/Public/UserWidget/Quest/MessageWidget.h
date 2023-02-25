// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Message;

public:
	void SetMessage(FText Text);
};

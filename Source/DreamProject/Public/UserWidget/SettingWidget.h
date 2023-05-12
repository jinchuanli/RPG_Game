// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UButton* SaveButton;

	class ARBaseCharacter* Playercharacter;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSaveButtonClicked();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestEnum.h"
#include "QuestCategoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UQuestCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* ExpandButton;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CategoriesName;
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* QuestBox;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere)
	EQuestStates CurrenState = EQuestStates::CurrentQuest;

	bool bExpand = false;
	
	UFUNCTION()
	void OnExpandButtonClicked();
public:
	void SetCategoryName(FText Text);
};

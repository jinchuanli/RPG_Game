// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestStruct.h"
#include "SubGoalWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USubGoalWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GoalText;

	//动态注册
	UFUNCTION()
	void SelectedButtonClicked();
public:
	UPROPERTY(meta=(BindWidget))
	class UButton* SelectButton;
	UPROPERTY(meta=(BindWidget))
	class UBorder* GoalBorder;
	
	FGoalInfo GoalInfo;  //子任务的信息
	class ABaseQuest* AssignedQuest;
	class UQuestWidget* QuestWidget;
	int GoalIndex;//表示子任务在总任务当中的索引是哪一个

	int HuntIndex;
	void Update();

	virtual void NativeConstruct() override;
};

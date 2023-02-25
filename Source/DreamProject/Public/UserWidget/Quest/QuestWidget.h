// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* QuestName;
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* SubGoalBox;


	class AQuestManager* QuestManager;
	class ABaseQuest* AssignedQuest;
	TArray<class USubGoalWidget*> SubGoalWidgets;
	class USubGoalWidget* SelectedSubGoalWidget;

	void GenerateSubWidgets();  //更新子任务
	void UpdateQuest();
	bool bCurrentQuest();
	void SelectSubGoal(class USubGoalWidget* ClickedSubGoalWidget);

	void OnQuestSelected(class USubGoalWidget* SubGoalWidget);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournal.generated.h"

class UQuestCategoryWidget;
class UTextBlock;
class UVerticalBox;
class UButton;
class UHorizontalBox;
class UScrollBox;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UQuestJournal : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UScrollBox* QuestList;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* QuestDetailsBox;
	UPROPERTY(meta=(BindWidget))
	UQuestCategoryWidget* CatCurrent;
	UPROPERTY(meta=(BindWidget))
	UQuestCategoryWidget* CatCompleted;
	UPROPERTY(meta=(BindWidget))
	UQuestCategoryWidget* CatFaild;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestCategory;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestRegion;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SuggestedLevel;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* RewardBox;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ExpReward;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldReward;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Description;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* QuestGoalBox;
	UPROPERTY(meta=(BindWidget))
	UButton* SelectButton;
	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* ButtonBox;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* ExpRewardBox;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* GoldRewardBox;

	virtual void NativeConstruct() override;
public:
	class ABaseQuest* SelectedQuest;
	class ARBaseCharacter* PlayerCharacter;

	//添加任务管理器
	class AQuestManager* QuestManager;

	void UpdateSuggestedLevelColor();
	void UpdateDescription();
	void GenereateSubGoals(); //生成子任务框

	void UpdateDetailWindow();

	//添加任务到分类框
	void AddQuestEntry(class ABaseQuest* Quest);
	TArray<class UQuestListEntry*> AllQuestListEntries;
	class UQuestListEntry* CurrentQuestListEntry;

	void QInitialize (class AQuestManager* FQuestManager);

	void OnQuestClicked(class UQuestListEntry* ClickedQuestListEntry);

	UFUNCTION()
	void OnSelectButtonClicked();
};

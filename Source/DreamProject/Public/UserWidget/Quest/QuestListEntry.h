// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListEntry.generated.h"

enum class ERegions;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UQuestListEntry : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RegionName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SuggestedLevel;
	UPROPERTY(meta=(BindWidget))
	class UButton* QuestButton;


	void Update();//更新任务的标题

	FString GetRegionEnumString(ERegions Region);

	virtual void NativeConstruct() override;
public:
	void UpdateLevelColor();
	class UQuestJournal* Journal;

	class ABaseQuest* AssignedQuest;

	void SetQuestName(FText QuestNameText);
	void SetRegionName(FText RegionNameText);
	void SetSuggestedLevel(FText LevelText);

	UFUNCTION()
	void OnQuestButtonClicked();
};

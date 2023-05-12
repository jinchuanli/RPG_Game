// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestJournal.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Other/StaticLibrary.h"
#include "Quest/BaseQuest.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/GoalEntryWidget.h"
#include "UserWidget/Quest/QuestCategoryWidget.h"
#include "UserWidget/Quest/QuestListEntry.h"
#include "UserWidget/Quest/QuestWidget.h"

#define LOCTEXT_NAMESPACE "QuestJournal"

void UQuestJournal::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this,&UQuestJournal::OnSelectButtonClicked);
}

void UQuestJournal::OnSelectButtonClicked()
{
	if(QuestManager->CurrentQuest != SelectedQuest)
	{
		QuestManager->SelectNewQuest(SelectedQuest,SelectedQuest->QuestWidget->SubGoalWidgets[0]);
	}
}

void UQuestJournal::UpdateSuggestedLevelColor()
{
	FLinearColor TempColor;
	TempColor = (PlayerCharacter->GetCurrentLevel() > SelectedQuest->QuestInfo.SuggestedLevel) ? FLinearColor::Green :
	((PlayerCharacter->GetCurrentLevel() == SelectedQuest->QuestInfo.SuggestedLevel) ? FLinearColor::White : FLinearColor::Red);
	SuggestedLevel->SetColorAndOpacity(TempColor);
}

void UQuestJournal::UpdateDescription()
{
	UE_LOG(LogTemp,Warning,TEXT("更新任务描述成功"));
	Description->SetText(FText::FromString(SelectedQuest->CurrentDescription.ToString().Replace(TEXT("/n"),TEXT("\n"))));
}

void UQuestJournal::GenereateSubGoals()
{
	QuestGoalBox->ClearChildren();
	for(FCompletedGoal CompletedGoal : SelectedQuest->CompletedSubGoals)
	{
		UGoalEntryWidget* GoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(),
			LoadClass<UGoalEntryWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_GoalEntry.WBP_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo = CompletedGoal.GoalInfo;
		GoalEntryWidget->Journal = this;
		GoalEntryWidget->GoalState = CompletedGoal.bSuccesful ? EGoalStates::Success : EGoalStates::Failed;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget);
	}
	//for(FGoalInfo GoalInfo : SelectedQuest->CurrentGoals)
	for(int i=0;i<SelectedQuest->CurrentGoals.Num();i++)
	{
		UGoalEntryWidget* GoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(),
			LoadClass<UGoalEntryWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_GoalEntry.WBP_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo = SelectedQuest->CurrentGoals[i];
		GoalEntryWidget->Journal = this;
		GoalEntryWidget->GoalState = EGoalStates::Current;
		GoalEntryWidget->HuntIndex = i;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget);
	}
	UE_LOG(LogTemp,Warning,TEXT("任务分类成功"));
}

void UQuestJournal::UpdateDetailWindow()
{
	if(SelectedQuest)
	{
		QuestName->SetText(SelectedQuest->QuestInfo.Name);
		QuestCategory->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<EQuestCategories>("EQuestCategories",SelectedQuest->QuestInfo.Categories)));
		FLinearColor TempColor;
		switch (SelectedQuest->QuestInfo.Categories)
		{
		case EQuestCategories::MainQuest:
			TempColor = FLinearColor(1,0.37,0,1);
			break;
		case EQuestCategories::SideQuest:
			TempColor = FLinearColor(0.45,0.47,1,1);
			break;
		case EQuestCategories::Event:
			TempColor = FLinearColor(1,0.19,0.13,1);
			break;
		default:
			break;
		}
		QuestCategory->SetColorAndOpacity(TempColor);

		QuestRegion->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<ERegions>("ERegions",SelectedQuest->QuestInfo.Regions)));

		SuggestedLevel->SetText(FText::AsNumber(SelectedQuest->QuestInfo.SuggestedLevel));

		UpdateSuggestedLevelColor();

		//更新任务的奖励
		if(SelectedQuest->QuestInfo.CompletionReward.Experience > 0)
		{
			ExpRewardBox->SetVisibility(ESlateVisibility::Visible);
			ExpReward->SetText(FText::Format(LOCTEXT("QuestJournal","+{0} Exp"),FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Experience)));
		}
		else
		{
			ExpRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if(SelectedQuest->QuestInfo.CompletionReward.Coin > 0)
		{
			GoldRewardBox->SetVisibility(ESlateVisibility::Visible);
			GoldReward->SetText(FText::Format(LOCTEXT("QuestJournal","+{0} Gold"),FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Coin)));
		}
		else
		{
			GoldRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if(SelectedQuest->QuestInfo.CompletionReward.Experience <= 0 && SelectedQuest->QuestInfo.CompletionReward.Coin <= 0)
		{
			RewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		//更新任务的描述
		UpdateDescription();

		//子任务
		GenereateSubGoals();

		//按钮
		if(SelectedQuest->CurrentState == EQuestStates::CurrentQuest)
		{
			ButtonBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ButtonBox->SetVisibility(ESlateVisibility::Collapsed);//Collapsed不占永局部空间，
		}
		
		QuestDetailsBox->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp,Warning,TEXT("任务细节面板显示"));
	}
	else
	{
		QuestDetailsBox->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp,Warning,TEXT("任务细节面板隐藏"));
	}
	UE_LOG(LogTemp,Warning,TEXT("更新任务界面成功"));
}

void UQuestJournal::AddQuestEntry(class ABaseQuest* Quest)
{
	UQuestListEntry* QuestListEntry = CreateWidget<UQuestListEntry>(GetWorld(),
		LoadClass<UQuestListEntry>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_QuestListEntry.WBP_QuestListEntry_C'")));
	QuestListEntry->Journal = this;
	QuestListEntry->AssignedQuest = Quest;
	Quest->ListEntryWidget = QuestListEntry;

	AllQuestListEntries.Add(QuestListEntry);
	switch (Quest->CurrentState)
	{
	case EQuestStates::CurrentQuest:
		CatCurrent->QuestBox->AddChild(QuestListEntry);
		break;
	case EQuestStates::CompletedQuest:
		CatCompleted->QuestBox->AddChild(QuestListEntry);
		break;
	case EQuestStates::FailedQuest:
		CatFaild->QuestBox->AddChild(QuestListEntry);
		break;
	default:
		break;
	}
}

void UQuestJournal::QInitialize(AQuestManager* FQuestManager)
{
	UE_LOG(LogTemp,Warning,TEXT("任务面板初始成功"));
	this->QuestManager = FQuestManager;
	PlayerCharacter = QuestManager->PlayerCharacter;
}

void UQuestJournal::OnQuestClicked(UQuestListEntry* ClickedQuestListEntry)
{
	if (ClickedQuestListEntry)
	{
		if(CurrentQuestListEntry)
		{
			CurrentQuestListEntry->SetIsEnabled(true);
		}
		CurrentQuestListEntry = ClickedQuestListEntry;
		SelectedQuest = CurrentQuestListEntry->AssignedQuest;
		UpdateDetailWindow();
		CurrentQuestListEntry->SetIsEnabled(false);
		//UE_LOG(LogTemp,Warning,TEXT("qqqqqqq"));
	}
	else
	{
		UpdateDetailWindow();
	}
}

#undef LOCTEXT_NAMESPACE

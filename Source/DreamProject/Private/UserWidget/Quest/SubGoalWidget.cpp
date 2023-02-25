// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/SubGoalWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Quest/BaseQuest.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/QuestWidget.h"

#define LOCTEXT_NAMESPACE "SubGoalWidget"

void USubGoalWidget::SelectedButtonClicked()
{
	if(QuestWidget->bCurrentQuest())
	{
		QuestWidget->SelectSubGoal(this);
	}
	else
	{
		QuestWidget->QuestManager->SelectNewQuest(AssignedQuest,this);
	}
}

void USubGoalWidget::Update()
{
	//HuntIndex = AssignedQuest->CurrentGoals.Find(GoalInfo);
	//通过遍历的方法在结构体中找到
	for(int i = 0;i < AssignedQuest->CurrentGoals.Num();i++)
	{
		if(GoalInfo == AssignedQuest->CurrentGoals[i])
		{
			HuntIndex = i;
			break;
		}
	}
	if(GoalInfo.bCustomGoal)
	{ //没有任务
		GoalText->SetText(GoalInfo.GoalText);
	}
	else//有任务
	{
		FText TempText;
		FText bS;
		switch (GoalInfo.Type)
		{
		case EGoalTypes::Custom:
			TempText = GoalInfo.GoalText;
			break;
		case EGoalTypes::Hunt:
			if(GoalInfo.AmountToHunt > 1)
			{
				bS = FText::FromString("s");
			}
			else
			{
				bS = FText::FromString("");
			}
			TempText = FText::Format(LOCTEXT("SubGoalWidget","Hunt{0}{1}:{2}/{3}"),
				GoalInfo.AdditionalName,
				bS,
				FText::AsNumber(AssignedQuest->CurrentHuntedAmount[HuntIndex]),
				FText::AsNumber(GoalInfo.AmountToHunt));
			break;
		case EGoalTypes::Find:
			TempText = FText::Format(LOCTEXT("SubGoalWidget","Find the {0}"),
				GoalInfo.AdditionalName);
			break;
		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("SubGoalWidget","Talk to {0}"),
				GoalInfo.AdditionalName);
			break;
		default:
			break;
		}
		GoalText->SetText(TempText);
	}

	for(int i = 0;i < AssignedQuest->QuestInfo.SubGoals.Num();i++)
	{
		if(AssignedQuest->QuestInfo.SubGoals[i] == GoalInfo)
		{
			GoalIndex = i;
		}
	}
}

void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//在屏幕上点击，需要进行动态注册
	SelectButton->OnClicked.AddDynamic(this,&USubGoalWidget::SelectedButtonClicked);
	Update();
}


#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/GoalEntryWidget.h"

#include "GameModeInfoCustomizer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/QuestJournal.h"

#define LOCTEXT_NAMESPACE "GoalEntryWidget"
void UGoalEntryWidget::Update()
{
	//更新文字
	if(GoalInfo.bCustomGoal)
	{
		SetGoalText(GoalInfo.GoalText);
		UE_LOG(LogTemp,Warning,TEXT("更新文字"));
	}
	else
	{
		FText TempText;
		FText bS;
		int TempHuntedAmount = 0;
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

			if(GoalState == EGoalStates::Failed)
			{
				TempHuntedAmount = 0;
			}
			else if(GoalState == EGoalStates::Success)
			{
				TempHuntedAmount = GoalInfo.AmountToHunt;
			}
			else
			{
				TempHuntedAmount = Journal->SelectedQuest->CurrentHuntedAmount[HuntIndex];
			}
			
			TempText = FText::Format(LOCTEXT("GoalEntryWidget","Hunt {0}{1}:{2}/{3}"),
				GoalInfo.AdditionalName,
				bS,
				FText::AsNumber(TempHuntedAmount),
				FText::AsNumber(GoalInfo.AmountToHunt));
			break;
		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("GoalEntryWidget","Talk to {0}"),GoalInfo.AdditionalName);
			break;
		case EGoalTypes::Find:
			TempText = FText::Format(LOCTEXT("GoalEntryWidget","Find the {0}"),GoalInfo.AdditionalName);
			break;
		default:
			break;
		}
		SetGoalText(TempText);
	}
	//切换显示的图标
	UTexture2D* TempTexture = nullptr;
	switch (GoalState)
	{
	case EGoalStates::Current:
		TempTexture = CurrentTexture;
		break;
	case EGoalStates::Success:
		TempTexture = SuccessTexture;
		break;
	case EGoalStates::Failed:
		TempTexture = FailedTexture;
		break;
	default:
		break;
	}
	if(TempTexture)
	{
		GoalStateImage->SetBrushFromTexture(TempTexture);
	}
}

void UGoalEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Warning,TEXT("NativeEntryWidget"));
	Update();
}

void UGoalEntryWidget::SetGoalText(FText Text)
{
	GoalText->SetText(Text);
}

#undef LOCTEXT_NAMESPACE
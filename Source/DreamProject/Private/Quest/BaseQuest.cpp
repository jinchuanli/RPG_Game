// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/BaseQuest.h"

#include "Components/VerticalBox.h"
#include "Quest/QuestManager.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Quest/QuestJournal.h"
#include "UserWidget/Quest/QuestListEntry.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/Quest/SubGoalWidget.h"

// Sets default values
ABaseQuest::ABaseQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseQuest::BeginPlay()
{
	Super::BeginPlay();
	CaculateGoalAmount();
}

void ABaseQuest::UpdateSubGoals()
{
	CurrentGoals.Empty();
	for(int i = 0;i < CurrentGoalIndices.Num();i++)
	{
		CurrentGoals.Add(QuestInfo.SubGoals[CurrentGoalIndices[i]]);
	}
}

void ABaseQuest::SetupStartingGoals()
{
	CurrentGoalIndices.Empty();
	CurrentGoalIndices = StartingSubGoalIndices;
	UpdateSubGoals();
	CurrentDescription = QuestInfo.Description;
	CurrentHuntedAmount.SetNum(CurrentGoalIndices.Num());
}

bool ABaseQuest::GoToNextSubGoal()
{
	if(CurrentGoalIndices.Max()+1 <= QuestInfo.SubGoals.Num())
	{
		int NextIndex = CurrentGoalIndices.Max()+1;
		CurrentGoalIndices.Empty();
		CurrentGoalIndices.Add(NextIndex);
		UpdateSubGoals();
		return true;
	}
	else
	{
		return false;
	}
}

bool ABaseQuest::bSelectInJournal()
{
	return (QuestManager->MainUserWidget->QuestJournal->SelectedQuest && QuestManager->MainUserWidget->QuestJournal->SelectedQuest == this);
}

// Called every frame
void ABaseQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseQuest::OnSubGoalCompleted(int SubGoalIndex,bool bSuccess)
{
	if(CurrentGoalIndices.Contains(SubGoalIndex))
	{
		CurrentGoalCount++;
		FGoalInfo CompletedGoal = QuestInfo.SubGoals[SubGoalIndex];
		
		if(CompletedGoal.bUpdateQuestDescription && bSuccess)
		{
			CurrentDescription = FText::FromString(CurrentDescription.ToString() + CompletedGoal.bUpdateDescription.ToString());
			if(bSelectInJournal())
			{
				QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
			}
		}
		
		CurrentGoals.Remove(CompletedGoal);

		int WidgetIndex = CurrentGoalIndices.Find(SubGoalIndex);
		QuestWidget->SubGoalWidgets[WidgetIndex]->RemoveFromParent();
		
		CurrentHuntedAmount.Remove(WidgetIndex);  //完成任务，移除索引
		
		QuestWidget->SubGoalWidgets.RemoveAt(WidgetIndex);//RemoveAt传递的是一个索引
		CurrentGoalIndices.Remove(SubGoalIndex);//Remove传递的是一个元素

		if(bSuccess)
		{
			//子任务成功完成
			CompletedSubGoals.Add(FCompletedGoal{SubGoalIndex,CompletedGoal,true});
			if(CompletedGoal.bFailMeansQuestFail)
			{
				CurrentMustSuccedCount++;
			}
			
			for(int i : CompletedGoal.FollowingSubGoalIndices)
			{
				CurrentGoalIndices.Add(i);
				CurrentGoals.Add(QuestInfo.SubGoals[i]);

				if(i > CurrentHuntedAmount.Num())
				{
					CurrentHuntedAmount.Add(0);
				}
			
				USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(),
					LoadClass<USubGoalWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_SubGoal.WBP_SubGoal_C'")));
				SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
				SubGoalWidget->AssignedQuest = this;
				SubGoalWidget->QuestWidget = QuestWidget;

				QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
				QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
			}
		}
		else//子任务失败
		{
			CompletedSubGoals.Add(FCompletedGoal{SubGoalIndex,CompletedGoal,false});
			if(CompletedGoal.bFailMeansQuestFail)
			{
				//子任务失败意味总任务失败
				CurrentState = EQuestStates::FailedQuest;
				QuestManager->bCurrentQuestFinished = true;
				for(int i : CurrentGoalIndices)
				{
					CompletedSubGoals.Add(FCompletedGoal{i,GoalAtIndex(i),false});
				}
				CurrentGoalIndices.Empty();
				CurrentHuntedAmount.Empty();
				CurrentGoals.Empty();

				ListEntryWidget->RemoveFromParent();
				QuestWidget->RemoveFromParent();
				QuestManager->OnQuestEnd(this);
				return true;
			}
			else
			{
				CompletedSubGoals.Add(FCompletedGoal{SubGoalIndex,CompletedGoal,false});
				for(int i : CompletedGoal.FollowingSubGoalIndices)
				{
					CurrentGoalIndices.Add(i);
					CurrentGoals.Add(QuestInfo.SubGoals[i]);

					if(i > CurrentHuntedAmount.Num())
					{
						CurrentHuntedAmount.Add(0);
					}
			
					USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(),
						LoadClass<USubGoalWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_SubGoal.WBP_SubGoal_C'")));
					SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
					SubGoalWidget->AssignedQuest = this;
					SubGoalWidget->QuestWidget = QuestWidget;

					QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
					QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
				}

				if(CompletedGoal.bUpdateQuestDescription)
				{
					CurrentDescription = FText::FromString(CurrentDescription.ToString() + CompletedGoal.bUpdateDescription.ToString());
					if(bSelectInJournal())
					{
						QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
					}
				}
			}
		}
		//总任务是否已经完成
		if(CurrentGoalCount >= TotalGoalAmount)
		{
			QuestManager->bCurrentQuestFinished = true;
			//所有的子任务都完成
			if(CurrentMustSuccedCount >= TotalMustSuccedAmount)
			{
				//所有必须要完成的任务都完成了
				CurrentState = EQuestStates::CompletedQuest;
			}
			else
			{
				CurrentState = EQuestStates::FailedQuest;
			}
			CurrentGoalIndices.Empty();
			CurrentHuntedAmount.Empty();
			CurrentGoals.Empty();

			ListEntryWidget->RemoveFromParent();
			QuestWidget->RemoveFromParent();
			QuestManager->OnQuestEnd(this);
			return true;
		}

		if(bSelectInJournal())
		{
			QuestManager->MainUserWidget->QuestJournal->GenereateSubGoals();
		}

		if(SubGoalIndex == SelectedSubGoalIndex && QuestWidget->SubGoalWidgets.Num() > 0)
		{
			QuestWidget->SelectSubGoal(QuestWidget->SubGoalWidgets[0]);
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

FGoalInfo ABaseQuest::GoalAtIndex(int Index)
{
	return QuestInfo.SubGoals[Index];
}

void ABaseQuest::CaculateGoalAmount()
{
	for(FGoalInfo TempGoalInfo : QuestInfo.SubGoals)
	{
		TotalGoalAmount++;
		if(TempGoalInfo.bFailMeansQuestFail)
		{
			TotalMustSuccedAmount++;
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManager.h"

#include "Character/RBaseCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Quest/BaseQuest.h"
#include "Quest/GoalActor.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/MiniMapWidget.h"
#include "UserWidget/Quest/QuestJournal.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/Quest/SubGoalWidget.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AQuestManager::SelectNewQuest(ABaseQuest* NewQuest, USubGoalWidget* SubGoalWidget)
{
	if(CurrentQuest)
	{
		CurrentQuest->QuestWidget->QuestName->SetIsEnabled(false);
		CurrentQuest->QuestWidget->SelectSubGoal(nullptr);
	}
	CurrentQuest = NewQuest;
	CurrentQuest->QuestWidget->OnQuestSelected(SubGoalWidget);
}

bool AQuestManager::AddNewQuest(TSubclassOf<ABaseQuest> QuestClass)
{
	if(QuestClass && !AllQuestClasses.Contains(QuestClass))
	{
		AllQuestClasses.Add(QuestClass);
		ABaseQuest* TempQuest = GetWorld()->SpawnActor<ABaseQuest>(QuestClass,FVector::ZeroVector,FRotator::ZeroRotator);
		CurrentQuestActors.Add(TempQuest);
		TempQuest->SetupStartingGoals();

		TempQuest->QuestManager = this;
		
		TempQuest->QuestWidget = MainUserWidget->AddQuestToList(TempQuest);
		TempQuest->QuestWidget->QuestManager = this;
		TempQuest->QuestWidget->UpdateQuest();

		MainUserWidget->QuestJournal->AddQuestEntry(TempQuest);

		if(CurrentQuestActors.Num() <= 1)
		{
			SelectNewQuest(TempQuest,TempQuest->QuestWidget->SubGoalWidgets[0]);
		}
		PlayerCharacter->PlayAddNewQuestAnim();//默认也要播放一次动画
		return true;
	}
	else
	{
		return false;
	}
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AQuestManager::GetDistanceToGoal()
{
	if(PlayerCharacter!=nullptr&&CurrentGoal!=nullptr)
		return FMath::FloorToInt(FVector::Dist2D(PlayerCharacter->GetActorLocation(), CurrentGoal->GetActorLocation())/100) ;
	else
	{
		return 0;
	}
	//return FMath::FloorToInt(FVector::Dist2D(PlayerCharacter->GetActorLocation(),CurrentGoal->GetActorLocation() / 100));
}

void AQuestManager::UpdateDirectionArrow()
{
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(),CurrentGoal->GetActorLocation());
	MainUserWidget->MiniMap->RotateDirectionArrow(Rotator.Yaw);
}

void AQuestManager::OnSwitchSubQuest()
{
	if(CurrentGoal)
	{
		CurrentGoal->Destroy();
	}
	CurrentSubGoal = CurrentQuest->QuestInfo.SubGoals[CurrentQuest->SelectedSubGoalIndex];
	if(CurrentSubGoal.GoalLocation.bHasLocation)
	{
		CurrentGoal = GetWorld()->SpawnActor<AGoalActor>(GoalActorClass,CurrentSubGoal.GoalLocation.Location,FRotator::ZeroRotator);
		CurrentGoal->InitializeMinimapIcon(CurrentSubGoal.bUseRadius,CurrentSubGoal.Radius,CurrentSubGoal.CircleColor);
		MainUserWidget->MiniMap->SetDistanceText(GetDistanceToGoal());
		UpdateDirectionArrow();
		if(GetDistanceToGoal() > ShowHintDistance)
		{
			MainUserWidget->MiniMap->SetGoalHintVisible(true);
		}
		else
		{
			MainUserWidget->MiniMap->SetGoalHintVisible(false);
		}
	}
	else
	{
		CurrentGoal = nullptr;
		MainUserWidget->MiniMap->SetGoalHintVisible(false);
	}
}

void AQuestManager::OnPlayerMove()
{
	if(CurrentGoal)
	{
		//UE_LOG(LogTemp,Warning,TEXT("玩家移动显示ui"))
		MainUserWidget->MiniMap->SetDistanceText(GetDistanceToGoal());
		UpdateDirectionArrow();
		if(GetDistanceToGoal() > ShowHintDistance)
		{
			MainUserWidget->MiniMap->SetGoalHintVisible(true);
		}
		else
		{
			MainUserWidget->MiniMap->SetGoalHintVisible(false);
		}
	}
}

void AQuestManager::OnEnemyKilled(TSubclassOf<class ANormalEnemy> Enemy)
{
	//for(ABaseQuest* CurrentQuests : CurrentQuestActors)
	for(int j = 0;j < CurrentQuestActors.Num();j++)
	{
		for(int i = 0;i < CurrentQuestActors[j]->CurrentGoals.Num();i++)
		{
			if(CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Hunt && CurrentQuestActors[j]->CurrentGoals[i].GoalClass == Enemy)
			{
				CurrentQuestActors[j]->CurrentHuntedAmount[i]++;
				if(CurrentQuestActors[j]->CurrentHuntedAmount[i] >= CurrentQuestActors[j]->CurrentGoals[i].AmountToHunt)
				{
					CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				}

				if(bCurrentQuestFinished)
				{
					bCurrentQuestFinished = false;
					break;
				}
				
				CurrentQuestActors[j]->QuestWidget->SubGoalWidgets[i]->Update();

				if(CurrentQuestActors[j]->bSelectInJournal())
				{
					MainUserWidget->QuestJournal->GenereateSubGoals();
				}
			}
		}
	}
}

//寻找任务完成
void AQuestManager::OnObjectFound(TSubclassOf<class ABaseQuestProp> Prop)
{
	//for(ABaseQuest* TempCurrentQuest : CurrentQuestActors)
	for(int j = 0; j < CurrentQuestActors.Num();j++)
	{
		for(int i = 0;i < CurrentQuestActors[j]->CurrentGoals.Num();i++)
		{
			if(CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Find && CurrentQuestActors[j]->CurrentGoals[i].GoalClass == Prop)
			{
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				// if(CurrentGoal)
				// {
				// 	CurrentGoal->Destroy();
				// 	CurrentGoal = nullptr;
				// 	//UE_LOG(LogTemp,Warning,TEXT("寻找任务完成，销毁UI"))
				// }
				if(bCurrentQuestFinished)
				{
					bCurrentQuestFinished = false;
					break;
				}
			}
		}
	}
}

void AQuestManager::OnTalkToNPC(TSubclassOf<class ABaseNPC> NPC, int NPCId)
{
	//for(ABaseQuest* TempCurrentQuest : CurrentQuestActors)
	for(int j = 0; j < CurrentQuestActors.Num();j++)
	{
		for(int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num();i++)
		{
			if(CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Talk && CurrentQuestActors[j]->CurrentGoals[i].GoalClass == NPC &&  CurrentQuestActors[j]->CurrentGoals[i].GoalId == NPCId)
			{
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				if(bCurrentQuestFinished)
				{
					bCurrentQuestFinished = false;
					break;
				}
			}
		}
	}
}

void AQuestManager::OnQuestEnd(ABaseQuest* Quest)
{
	CurrentQuestActors.Remove(Quest);
	switch (Quest->CurrentState)
	{
	case EQuestStates::CompletedQuest:
		CompletedQuestActors.Add(Quest);
		break;
	case EQuestStates::FailedQuest:
		FailedQuestActors.Add(Quest);
		break;
	default:
		break;
	}
	MainUserWidget->QuestJournal->AddQuestEntry(Quest);
	if(CurrentQuest == Quest)
	{
		CurrentQuest = nullptr;
		if(CurrentGoal)  //地图上目标的UI显示
		{
			CurrentGoal->Destroy();
			CurrentGoal = nullptr;
			//UE_LOG(LogTemp,Warning,TEXT("摧毁当前目标的实例"));
		}
		MainUserWidget->MiniMap->DistanceBox->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->MiniMap->DirectionArrow->SetVisibility(ESlateVisibility::Hidden);
		if(CurrentQuestActors.Num() > 0)
			if(CurrentQuestActors[0])
			{
				SelectNewQuest(CurrentQuestActors[0],CurrentQuestActors[0]->QuestWidget->SubGoalWidgets[0]);
			}
	}
	if(Quest->bSelectInJournal())
	{
		MainUserWidget->QuestJournal->SelectedQuest = nullptr;
		MainUserWidget->QuestJournal->OnQuestClicked(nullptr);
	}
	if(Quest->CurrentState == EQuestStates::CompletedQuest)
	{
		PlayerCharacter->IncreaseCurrentExp(Quest->QuestInfo.CompletionReward.Experience);
		PlayerCharacter->IncreaseCoin(Quest->QuestInfo.CompletionReward.Coin);
	}
}
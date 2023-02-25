// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStruct.h"
#include "GameFramework/Actor.h"
#include "BaseQuest.generated.h"

UCLASS()
class DREAMPROJECT_API ABaseQuest : public AActor   //子任务的管理
{
	GENERATED_BODY()
	
public:	 //管理的子任务
	// Sets default values for this actor's properties
	ABaseQuest();

	UPROPERTY(EditAnywhere,Category=QuestInformation)
	FQuestInfo QuestInfo;//任务的信息
	UPROPERTY(EditAnywhere,Category=QuestInformation)
	TArray<int> StartingSubGoalIndices = {0};//初始的子任务的索引，初始的时候，可能有一个，也可能有多个
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<int> CurrentGoalIndices; //当前子任务的索引

	int SelectedSubGoalIndex;//当前选择的要完成的任务的索引
	TArray<int> CurrentHuntedAmount;//当前杀死的怪物的数量

	EQuestStates CurrentState;

	FText CurrentDescription;

	class UQuestListEntry* ListEntryWidget;

	TArray<FGoalInfo> CurrentGoals;//当前的所有的任务
	void UpdateSubGoals();//更新所有的子任务
	void SetupStartingGoals();//设置初始的目标
	bool GoToNextSubGoal();//当前的子任务完成，开始进行下一个子任务

	bool bSelectInJournal();
	class AQuestManager* QuestManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UQuestWidget* QuestWidget;//所对应的任务界面的UI

	//管理已经完成的子任务
	UPROPERTY(VisibleAnywhere,Category=QuestInformation)
	TArray<FCompletedGoal> CompletedSubGoals;

	bool OnSubGoalCompleted(int SubGoalIndex,bool bSuccess);

	FGoalInfo GoalAtIndex(int Index);


	int TotalGoalAmount;
	int CurrentGoalCount;
	int TotalMustSuccedAmount;
	int CurrentMustSuccedCount;

	void CaculateGoalAmount();
};

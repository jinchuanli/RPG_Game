// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestEnum.h"
#include "UObject/NoExportTypes.h"
#include "QuestStruct.generated.h"

USTRUCT()
struct FQuestReward   //任务完成后的奖励
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int Experience;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int Coin;
};

USTRUCT()
struct FGoalLocation
{
	GENERATED_BODY()
//目标位置信息
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	bool bHasLocation;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FVector Location;
};

USTRUCT()
struct FGoalInfo//子任务信息
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	EGoalTypes Type = EGoalTypes::Custom;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	bool bCustomGoal;  //是否有任务

	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FText GoalText;  //任务的记录描述
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FText AdditionalName;//任务作用的目标
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int AmountToHunt;//要杀死的怪物的数量
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FGoalLocation GoalLocation; //目标的位置
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	bool bUpdateQuestDescription; //当前任务完成之后，会有下一个任务的更新提示
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FText bUpdateDescription;//更新任务的描述
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	TArray<int> FollowingSubGoalIndices; //当前子任务完成之后接下来的子任务索引
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	bool bUseRadius;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	float Radius;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FLinearColor CircleColor;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	TSubclassOf<AActor> GoalClass;//作用的目标
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int GoalId;

	bool bFailMeansQuestFail;

	bool operator==(const FGoalInfo& GoalInfo)
	{
		return (GoalInfo.Type == this->Type &&
			GoalInfo.bCustomGoal == this->bCustomGoal &&
			GoalInfo.GoalText.EqualTo(this->GoalText) &&
			GoalInfo.AdditionalName.EqualTo(this->AdditionalName) &&
			GoalInfo.AmountToHunt == this->AmountToHunt &&
			GoalInfo.GoalLocation.bHasLocation == this->GoalLocation.bHasLocation &&
			GoalInfo.GoalLocation.Location == this->GoalLocation.Location &&
			GoalInfo.bUpdateQuestDescription == this->bUpdateQuestDescription &&
			GoalInfo.bUpdateDescription.EqualTo(this->bUpdateDescription )&&
			GoalInfo.FollowingSubGoalIndices == this->FollowingSubGoalIndices &&
			GoalInfo.bUseRadius == this->bUseRadius &&
			GoalInfo.Radius == this->Radius &&
			GoalInfo.CircleColor == this->CircleColor &&
			GoalInfo.GoalClass == this->GoalClass &&
			GoalInfo.GoalId == this->GoalId &&
			GoalInfo.bFailMeansQuestFail == this->bFailMeansQuestFail);
	}
};

USTRUCT()
struct FQuestInfo   //总的任务
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FText Name;
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	EQuestCategories Categories;//任务种类
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FText Description;//任务描述
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	ERegions Regions;//任务的区域
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	FQuestReward CompletionReward;  //完成的奖励
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int SuggestedLevel;//建议的等级
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	float Difficulty;//任务的难度
	UPROPERTY(EditAnywhere,Category=QuestInfo)
	TArray<FGoalInfo> SubGoals;//包括那些子任务
};

USTRUCT()
struct FCompletedGoal   //总的任务
{
	GENERATED_BODY()
	int GoalIndex;
	FGoalInfo GoalInfo;
	bool bSuccesful;
};
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UQuestStruct : public UObject
{
	GENERATED_BODY()
	
};

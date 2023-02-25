// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStruct.h"
#include "GameFramework/Actor.h"
#include "QuestManager.generated.h"

UCLASS()
class DREAMPROJECT_API AQuestManager : public AActor  //管理的总任务，单个任务的管理
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	class ARBaseCharacter* PlayerCharacter;
	class UMainUserWidget* MainUserWidget;
	class ABaseQuest* CurrentQuest;//当前处理的任务

	TArray<TSubclassOf<class ABaseQuest>>AllQuestClasses;
	TArray<class ABaseQuest*> CurrentQuestActors;//保存的是所有的任务生成的实例
	TArray<class ABaseQuest*> CompletedQuestActors;
	TArray<class ABaseQuest*> FailedQuestActors;
	
	FGoalInfo CurrentSubGoal;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category=Quest)
	float ShowHintDistance;
public:	
	void SelectNewQuest(class ABaseQuest* NewQuest,class USubGoalWidget* SubGoalWidget);//任务的选择

	UFUNCTION(BlueprintCallable)
	bool AddNewQuest(TSubclassOf<class ABaseQuest> QuestClass);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AGoalActor* CurrentGoal;  //生成实例

	UPROPERTY(EditAnywhere,Category=Quest)
	TSubclassOf<class AGoalActor> GoalActorClass;  //生成一个类
	
	float GetDistanceToGoal();

	void UpdateDirectionArrow();

	void OnSwitchSubQuest();
	void OnPlayerMove();

	void OnEnemyKilled(TSubclassOf<class ANormalEnemy> Enemy);

	//寻找物品的任务
	void OnObjectFound(TSubclassOf<class ABaseQuestProp> Prop);\

	//与NPC谈话
	void OnTalkToNPC(TSubclassOf<class ABaseNPC>NPC,int NPCId);

	void OnQuestEnd(class ABaseQuest* Quest);

	bool bCurrentQuestFinished;  //标识器，标识任务是否已经结束
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ANormalEnemyController : public AAIController
{
	GENERATED_BODY()
	ANormalEnemyController();
	//
	class ANormalEnemy* EnemyPawn;

	virtual void OnPossess(APawn* InPawn) override;

	//
	//是否正在巡逻
	class UNavigationSystemV1* NavSys;  //导航网格
public:
	bool bIsPatrolling = true;
	void Patrol();

	//
private:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;
	FTimerHandle TimerHandle_Patrol;
	void DetectedPatrol();  //判断是否可以继续巡逻
	public:
	//95
	float PatrolWalkSpeed = 200.f; //巡逻的时候移动速度
	float AggoredWalkSpeed = 600.f; //追逐的时候的移动速度

	AActor* TargetActor;//要攻击或追逐的对象
	bool bIsRunningBack = true; //判断状态是否是追逐目标但目标脱离了的状态
	int CurrentAttackIndex = 0; //当前动画的索引
	private:
	class UAnimMontage* CurrentAttackMontage;//当前正在播放的攻击动画

	

	//96
	float AttackRange = 300.f;//攻击范围
	int GetNextAnimtionIndex();  //得到下一个要播放动画的索引，主要是来判断动画数组到头的时候，将索引重新置为0
	bool bInAttackRange();  //判断目标是否在敌人的攻击范围之类
public:
	//97
	void PerFromAttack();//执行攻击的函数
	UAnimInstance* AnimInstance;
private:
	//98
	//攻击动画完成之后可以继续移动
	void OnAnimPlayOver();
	FTimerHandle TimerHandle_OnAnimPlayOver; //动画是否播放完了

	//99
public:
	void OnAggroedPulled(AActor* Target); //当发现目标之后，追逐目标,进入狂暴的状态；
	bool bWasAggroed = false; //是否处在狂暴的状态
private:
	void CacualteTargetDistance();
	float MaxDistanceToFollowTarget = 1500.f;//能够跟随的最大距离

	//100
	void OnReset(); // 当追逐目标，当目标脱离，回到初始位置，置为初始状态

	//101
	FTimerHandle TimerHandle_CalDis;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnemyController.h"
#include "H:\DreamProject\Source\DreamProject\Public\Enemy\NormalEnemy.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet//KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

ANormalEnemyController::ANormalEnemyController()
{
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}

void ANormalEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	EnemyPawn = Cast<ANormalEnemy>(InPawn);
	AnimInstance = EnemyPawn->GetMesh()->GetAnimInstance();
	EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;
}

//控制巡逻
void ANormalEnemyController::Patrol()
{
	const float SearchRadius = 1000.f;  //搜索半径
	if(NavSys)
	{
		FNavLocation RandomPt;
		bool bFound = NavSys->GetRandomReachablePointInRadius(EnemyPawn->GetActorLocation(),SearchRadius,RandomPt);
		if(bFound)
		{
			MoveToLocation(RandomPt.Location);
		}
	}
}

void ANormalEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if(bIsRunningBack)
	{
		bIsRunningBack = false;
		bIsPatrolling = true;
		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;
	}
	if(bInAttackRange())
	{
		PerFromAttack();
		return;
	}
	if(bIsPatrolling)
	{
		float ThinkTime = FMath::RandRange(1,4);
		GetWorldTimerManager().SetTimer(TimerHandle_Patrol,this,&ANormalEnemyController::DetectedPatrol,ThinkTime);
	}
}

void ANormalEnemyController::DetectedPatrol()
{
	if(bIsPatrolling)
	{
		Patrol();
	}
}

int ANormalEnemyController::GetNextAnimtionIndex()
{
	if(CurrentAttackIndex + 1 >= EnemyPawn->AttackAnimations.Num())
	{
		return 0;
	}
	else
	{
		return (CurrentAttackIndex + 1);
	}
}

bool ANormalEnemyController::bInAttackRange()
{
	if(TargetActor)
	{
		return (EnemyPawn->GetDistanceTo(TargetActor) <= AttackRange);
	}
	else
	{
		return false;
	}
}

void ANormalEnemyController::PerFromAttack()
{
	if(!EnemyPawn->GetbDead() && !bIsRunningBack && bWasAggroed)
	{
		bIsPatrolling = false;
		EnemyPawn->GetCharacterMovement()->StopMovementImmediately();
		if(TargetActor)
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(EnemyPawn->GetActorLocation(),TargetActor->GetActorLocation());
			EnemyPawn->SetActorRotation(Rotation);
		}
		//播放攻击动画
		CurrentAttackMontage = EnemyPawn->AttackAnimations[CurrentAttackIndex];
		AnimInstance->Montage_Play(CurrentAttackMontage);
		GetWorldTimerManager().SetTimer(TimerHandle_OnAnimPlayOver,this,&ANormalEnemyController::OnAnimPlayOver,CurrentAttackMontage->SequenceLength,false);
	}
}

void ANormalEnemyController::OnAnimPlayOver()
{
	CurrentAttackMontage = nullptr;
	CurrentAttackIndex = GetNextAnimtionIndex();
	if(bInAttackRange())
	{
		PerFromAttack();
	}
	else
	{
		MoveToActor(TargetActor);
	}
}

void ANormalEnemyController::OnAggroedPulled(AActor* Target)
{
	if(!bWasAggroed && !bIsRunningBack)
	{
		bWasAggroed = true;
		TargetActor = Target;
		bIsPatrolling = false;

		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = AggoredWalkSpeed;

		GetWorldTimerManager().SetTimer(TimerHandle_CalDis,this,&ANormalEnemyController::CacualteTargetDistance,1.f,true);
		if(bInAttackRange())
		{
			PerFromAttack();
		}
		else
		{
			MoveToActor(TargetActor);
		}
	}
}

void ANormalEnemyController::CacualteTargetDistance()
{
	if(TargetActor)
	{
		if(FVector::Dist(EnemyPawn->GetActorLocation(),TargetActor->GetActorLocation()) > MaxDistanceToFollowTarget)
		{
			OnReset();
		}
	}
}

void ANormalEnemyController::OnReset()
{
	AnimInstance->Montage_Stop(0.f);
	bIsRunningBack = true;
	EnemyPawn->CurrentHealth = EnemyPawn->TotalHealth;
	EnemyPawn->UpdateHealthBar();
	CurrentAttackIndex = 0;
	GetWorldTimerManager().ClearTimer(TimerHandle_Patrol);
	GetWorldTimerManager().ClearTimer(TimerHandle_OnAnimPlayOver);
	GetWorldTimerManager().ClearTimer(TimerHandle_CalDis);
	TargetActor = nullptr;
	bWasAggroed = false;
	MoveToLocation(EnemyPawn->StartLocation);
}


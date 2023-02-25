// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/MissileSkill.h"
#include "Character/RBaseCharacter.h"
#include "Enemy/NormalEnemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Skill/MagicProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

bool AMissileSkill::bInAttackRange()
{
	//判断是否在攻击的范围之类
	return (PlayerReference->GetDistanceTo(PlayerReference->SelectedEnemy) <= GetCurrentStage().Range);
}

void AMissileSkill::OnTryCastSpell()
{
	if(PlayerReference->SelectedEnemy)
	{
		Super::OnTryCastSpell();
	}
}

void AMissileSkill::InitializeSpellCast()
{
	if(bInAttackRange())//如果在攻击的范围之类
	{
		Super::InitializeSpellCast();
	}
	else//移动向敌人
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Caculate,this,&AMissileSkill::CaculateDistanceToEnemy,0.1f,true);
		PlayerReference->MissileSkill = this;
	}
}

void AMissileSkill::OnSpellCast()
{
	//使角色面对向目标
	PlayerReference->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerReference->GetActorLocation(),PlayerReference->SelectedEnemy->GetActorLocation()));
	Super::OnSpellCast();
}

void AMissileSkill::OnSkillNotify()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	//技能的发射位置和旋转
	GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectile,PlayerReference->GetMesh()->GetSocketLocation("Muzzle_01"),
		PlayerReference->GetMesh()->GetSocketRotation("Muzzle_01"),Params);
}

void AMissileSkill::CaculateDistanceToEnemy()
{
	if(PlayerReference->SelectedEnemy)
	{
		if(bInAttackRange())
		{
			InitializeSpellCast();
			StopCaculate();
		}
		else //距离不够移动向目标以达到距离足够
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(Cast<AController>(PlayerReference->PC),PlayerReference->SelectedEnemy);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	}
}

void AMissileSkill::StopCaculate()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	PlayerReference->MissileSkill = nullptr;
}

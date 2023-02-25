// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEnum.generated.h"

//技能作用的对象
UENUM()
enum class ETargetTypes:uint8
{
	Self,
	SelectedEnemy,
	SelectedArea,
	AreaAroundSelf,
	Missile  // 子弹类型
};

//伤害类型
UENUM()
enum class EDamageType:uint8
{
	Magic,
	Physical
};

//效果增加和效果削弱
UENUM()
enum class EEffectiveness:uint8
{
	Effective,
	SuperEffective,
	LowEffective
};
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USkillEnum : public UObject
{
	GENERATED_BODY()
	
};

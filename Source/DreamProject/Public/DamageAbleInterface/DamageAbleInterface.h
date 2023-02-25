// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/SkillEnum.h"
#include "UObject/interface.h"
#include "DamageAbleInterface.generated.h"

class ABaseElement;
class ABaseSkill;

UINTERFACE()
class UDamageAbleInterface:public UInterface
{
	GENERATED_BODY()
};

//接口
class IDamageAbleInterface
{
	GENERATED_BODY()
public:
	virtual void OnReceiveDamage(float BaseDamage,EDamageType Type,TSubclassOf<ABaseElement> Element,int CritChance,AActor* Attacker,ABaseSkill* Spell) = 0;  //需要他变成一个接口一定不要忘记等于0
};


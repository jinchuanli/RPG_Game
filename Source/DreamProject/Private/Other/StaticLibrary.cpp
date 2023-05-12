// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/StaticLibrary.h"
#include "Character/Skill/BaseElement.h"
#include "Enemy/NormalEnemy.h"
#include "Character/Skill/SkillEnum.h"
#include "Enemy/Boss.h"

float UStaticLibrary::CaculateFinalDamage(float BaseDamage, int CritChance, TSubclassOf<ABaseElement> AttackerElement,TSubclassOf<ABaseElement> DefenderElement)
{
	float CurrentDamage = BaseDamage*FMath::RandRange(0.9f,1.1f);
	EEffectiveness LocalEffectiveness;
	if(AttackerElement && DefenderElement)
	{
		if(DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Weakness.Contains(AttackerElement))
		{
			LocalEffectiveness = EEffectiveness::LowEffective;
		}
		else if(DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Enhance.Contains(AttackerElement))
		{
			LocalEffectiveness = EEffectiveness::SuperEffective;
		}
		else
		{
			LocalEffectiveness = EEffectiveness::Effective;
		}
	}
	else
	{
		LocalEffectiveness = EEffectiveness::Effective;
	}
	switch (LocalEffectiveness)
	{
	case EEffectiveness::Effective:
		CurrentDamage = CurrentDamage * 1;
		break;
		
	case EEffectiveness::SuperEffective:
		CurrentDamage = CurrentDamage * 1.5f;
		break;

	case EEffectiveness::LowEffective:
		CurrentDamage = CurrentDamage * 0.7f;
		break;

	default:
		break;
	}

//暴击率
	if(FMath::RandHelper(101) <= CritChance)
	{
		CurrentDamage = CurrentDamage * 2;
	}
	
	return CurrentDamage;
}


bool UStaticLibrary::bIsEnemy(AActor* Actor)
{
	return (Actor->GetClass() == ANormalEnemy::StaticClass()
		|| Actor->GetClass()->IsChildOf(ANormalEnemy::StaticClass())
		|| Actor->GetClass() == ABoss::StaticClass()
		|| Actor->GetClass()->IsChildOf(ABoss::StaticClass()));
}


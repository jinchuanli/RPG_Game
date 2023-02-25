// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/BuffSkill.h"

#include "Components/Image.h"
#include "UserWidget/BuffWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"

void ABuffSkill::ApplyEffect()
{
}

void ABuffSkill::RemoveEffect()
{
}

void ABuffSkill::ResetBuff()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_UpdateEffect);
	BuffDuration = GetCurrentStage().CastingTime;
	OnActive();
}

void ABuffSkill::OnActive()
{
	BuffWidgetRef = PlayerReference->AddBuff(this);
	BuffDuration = GetCurrentStage().CastingTime;
	BuffWidgetRef->CoolDownImage->SetVisibility(ESlateVisibility::Visible);
	BuffWidgetRef->CoolDownTime->SetVisibility(ESlateVisibility::Visible);
	RemainingTime = BuffDuration;
	GetWorldTimerManager().SetTimer(TimerHandle_UpdateEffect,this,&ABuffSkill::OnUpdateBuff,0.1f,true);
}

void ABuffSkill::OnDeActive()
{
	RemoveEffect();
	PlayerReference->RemoveBuff(this);
	BuffWidgetRef = nullptr;
}

void ABuffSkill::OnUpdateBuff()
{
	if(RemainingTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_UpdateEffect);
		OnDeActive();
	}
	else
	{
		ApplyEffect();
		RemainingTime -= 0.1f;
		BuffWidgetRef->CoolDownTime->SetText(FText::AsNumber(FMath::RoundToZero(RemainingTime)));
		BuffWidgetRef->CoolDownMat->SetScalarParameterValue("Percent",RemainingTime / BuffDuration);
	}
}

void ABuffSkill::OnSkillNotify()
{
	Super::OnSkillNotify();
	if(PlayerReference->Buffs.Contains(this))
	{
		ResetBuff();
	}
	else
	{
		OnActive();
	}
}

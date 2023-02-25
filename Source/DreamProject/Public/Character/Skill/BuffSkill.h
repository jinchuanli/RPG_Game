// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "BuffSkill.generated.h"

class UBuffWidget;
class UTexture2D;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ABuffSkill : public ABaseSkill
{
	GENERATED_BODY()
	FTimerHandle TimerHandle_UpdateEffect;
public:
	UBuffWidget* BuffWidgetRef;
	UPROPERTY(EditAnywhere)
	UTexture2D* BUffIcon;

	float BuffDuration;//持续时间
	float RemainingTime;
	

	virtual void ApplyEffect();//添加buff效果
	virtual void RemoveEffect();//移除buff效果
	
	virtual void ResetBuff();//重置BUff效果
	virtual void OnActive();//buff激活，这个技能的入口，除了applayEffect之外，还要完成添加buff之类的事情
	virtual void OnDeActive();//buff失活
	virtual void OnUpdateBuff();//在定时器中持续调用的函数
	virtual void OnSkillNotify() override; //重写通知的函数
};

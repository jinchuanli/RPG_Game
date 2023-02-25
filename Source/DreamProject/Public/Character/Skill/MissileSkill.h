// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "MissileSkill.generated.h"

class UParticleSystem;
class AMagicProjectile;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AMissileSkill : public ABaseSkill
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<AMagicProjectile> MagicProjectile; //持有子弹类
	bool bInAttackRange(); //返回是否在攻击的范围之类
	virtual void OnTryCastSpell() override; //尝试释放技能
	virtual void InitializeSpellCast() override;//初始技能释放,来判断魔法值是否足够
	virtual void OnSpellCast() override;//技能正在释放的时候，播放释放技能的动画
	
	virtual void OnSkillNotify() override; //动画通知的函数
	void CaculateDistanceToEnemy();

	FTimerHandle TimerHandle_Caculate;
public:
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* MissileEffect; //发射的子弹所持有的特效
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* ImpactEffect;//子弹打到目标所播放的特效

	void StopCaculate();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReleaseSkillProjectile.h"
#include "Character/Skill/BaseSkill.h"
#include "ReleaseSkill.generated.h"

class UParticleSystem;
class AMagicProjectile;
class UNiagaraSystem;
class USoundBase;

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AReleaseSkill : public ABaseSkill
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<AReleaseSkillProjectile> MagicProjectile;

	virtual void OnTryCastSpell() override;
	virtual void InitializeSpellCast() override;
	virtual void OnSpellCast() override;

	virtual void OnSkillNotify() override;
	virtual void OnPlaySound() override;

public:
	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* MissileEffect; //发射的子弹所持有的特效
	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* ImpactEffect;//子弹打到目标所播放的特效
	UPROPERTY(EditAnywhere,Category=Effect)
	USoundBase* sound;

	FTimerHandle TimerHandle_ResetOrient;
	void ResetOrient();
};

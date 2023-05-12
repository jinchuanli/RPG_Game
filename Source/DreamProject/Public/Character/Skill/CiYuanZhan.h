// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "CiYuanZhan.generated.h"

class UNiagaraSystem;
class USoundBase;

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ACiYuanZhan : public ABaseSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,Category=Skill)
	UNiagaraSystem* initiativeEffect;
	UPROPERTY(EditAnywhere,Category=Skill)
	UNiagaraSystem* passivityEffect;
	
	
	UPROPERTY(EditAnywhere,Category=Skill)
	USoundBase* StartSound;
	UPROPERTY(EditAnywhere,Category=Skill)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere,Category=Skill)
	int32 NumOfDamage;

public:
	virtual void OnSkillNotify() override;
	
};

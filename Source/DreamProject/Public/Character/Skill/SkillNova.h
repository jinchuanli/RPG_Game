// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "SkillNova.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ASkillNova : public ABaseSkill
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=Particle)
	class UParticleSystem* ParticleSystem;


	
	void OnSkillNotify() override;
	
};

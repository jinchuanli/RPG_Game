// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BuffSkill.h"
#include "BuffSkill_Health.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ABuffSkill_Health : public ABuffSkill
{
	GENERATED_BODY()
	virtual void ApplyEffect() override;
};

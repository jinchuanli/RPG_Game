// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "Evade.generated.h"

class UNiagaraSystem;
class USoundBase;
class UTimelineComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AEvade : public ABaseSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,Category=Skill)
	UNiagaraSystem* InitiativeEffect;

	UPROPERTY(EditAnywhere,Category=Skill)
	USoundBase* StartSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Curve)
	class UTimelineComponent* MyTimeLine;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Curve)
	class UCurveFloat* Curver;
	
	FVector StartLocation;
	FVector TargetLocation;
	class UCapsuleComponent* CapsuleComponent;
public:
	AEvade();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTimeLineTick(float Output);

	UFUNCTION()
	void OnTimeLineEndEvent();
	
	virtual void OnSkillNotify() override;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "Components/TimelineComponent.h"
#include "TuCi.generated.h"

class UNiagaraSystem;
class USoundBase;

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ATuCi : public ABaseSkill
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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Curve)
	class UTimelineComponent* MyTimeLine;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Curve)
	class UCurveFloat* Curver;

	FVector StartLocation;
	FVector EndLocation;

	class UCapsuleComponent* CapsuleComponent;
public:
	ATuCi();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTimeLineTick(float Output);
	UFUNCTION()
	void OnTimeLineEndEvent();
	
	virtual void OnSkillNotify() override;
	
};

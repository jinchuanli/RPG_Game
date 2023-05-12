// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=RPGAnim)
	bool IsIdle;
	FTimerHandle TimerHandle_Delay;

public:
	UFUNCTION(BlueprintCallable,Category=RPGAnim)
	void ChangIsIdle();
};

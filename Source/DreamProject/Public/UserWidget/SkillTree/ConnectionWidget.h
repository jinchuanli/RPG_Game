// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionWidget.generated.h"

/**
 * 用来链接两个技能的连线
 */
UCLASS()
class DREAMPROJECT_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UImage* Line;  //连线，没有前置技能时，A值为0.2，有时A为1

public:
	bool bActive;
	TSubclassOf<class ABaseSkill> ForSpell;  //需要的前置技能
	
	void Activate();
};

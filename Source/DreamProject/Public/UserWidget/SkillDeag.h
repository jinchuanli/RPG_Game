// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDeag.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USkillDeag : public UUserWidget
{
	GENERATED_BODY()

	//绑定Icon
	UPROPERTY(meta=(BindWidget))
	class UImage* SkillIcon;

public:
	void SetSkillTexture(class UTexture2D* Texture);
};

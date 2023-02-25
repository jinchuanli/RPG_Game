// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/ConnectionWidget.h"

#include "Components/Image.h"

void UConnectionWidget::Activate()
{
	if(!bActive)
	{
		bActive = true;
		Line->SetOpacity(0.7f);
		
	}
}

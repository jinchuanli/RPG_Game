// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillDeag.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

void USkillDeag::SetSkillTexture(UTexture2D* Texture)
{
	SkillIcon->SetBrushFromTexture(Texture);
}

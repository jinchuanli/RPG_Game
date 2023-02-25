// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BuffWidget.h"
#include "Components/Image.h"

void UBuffWidget::NativeConstruct()
{
	CoolDownMat = CoolDownImage->GetDynamicMaterial();
	BuffIcon->SetBrushFromTexture(BuffTexture);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MiniMapWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMiniMapWidget::RotateDirectionArrow(float Angle)
{
	DirectionArrow->SetRenderTransformAngle(Angle);
	
}

void UMiniMapWidget::SetDistanceText(float Distance)
{
	DistanceText->SetText(FText::AsNumber(Distance));
}

void UMiniMapWidget::SetGoalHintVisible(bool bVisible)
{
	if(bVisible)
	{
		DirectionArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);//设置显示
		DistanceBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		DirectionArrow->SetVisibility(ESlateVisibility::Hidden);//设置隐藏
		DistanceBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

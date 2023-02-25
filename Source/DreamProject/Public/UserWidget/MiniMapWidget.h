// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DistanceText;
public:
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* DistanceBox;
	UPROPERTY(meta=(BindWidget))
	class UImage* DirectionArrow;
	void RotateDirectionArrow(float Angle);
	void SetDistanceText(float Distance);
	void SetGoalHintVisible(bool bVisible);
};

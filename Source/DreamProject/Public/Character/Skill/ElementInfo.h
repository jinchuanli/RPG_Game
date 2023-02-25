// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Math/Color.h"
#include "ElementInfo.generated.h"


class ABaseElement;

//元素的信息
USTRUCT()
struct FElementInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,Category="ElementData")
	FText Name;
	UPROPERTY(EditAnywhere,Category="ElementData")
	class UTexture2D* Icon;//元素图标
	UPROPERTY(EditAnywhere,Category="ElementData")
	FLinearColor Color;//元素的颜色
	UPROPERTY(EditAnywhere,Category="ElementData")
	TArray<TSubclassOf<ABaseElement>> Enhance; //增强效果
	UPROPERTY(EditAnywhere,Category="ElementData")
	TArray<TSubclassOf<ABaseElement>> Weakness; //虚弱效果
};

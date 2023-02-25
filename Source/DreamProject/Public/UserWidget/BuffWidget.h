// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UBuffWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UImage* BuffIcon;
	UPROPERTY(meta=(BindWidget))
	UImage* CoolDownImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CoolDownTime;

	UTexture2D* BuffTexture;
	UMaterialInstanceDynamic* CoolDownMat;
	virtual void NativeConstruct() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/interface.h"
#include "SelectAbleInterface.generated.h"

UINTERFACE()
class USelectAbleInterface : public UInterface
{
	GENERATED_BODY()
};

class ISelectAbleInterface
{
	GENERATED_BODY()
public:
	virtual void OnSelected(class ARBaseCharacter* Character) = 0; // 选中的时候
	virtual void OnSelectionEnd(class ARBaseCharacter* Character) = 0; // 选择离开后调用
};
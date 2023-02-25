// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/interface.h"
#include "InterationInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class UInterationInterface : public UInterface
{
	GENERATED_BODY()
};

class IInterationInterface
{
	GENERATED_BODY()
public:
	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) = 0;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) = 0;
	virtual void OnIteractWith(class ARBaseCharacter* Character) = 0;
};

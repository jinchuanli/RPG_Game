// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MiniCam.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AMiniCam : public ASceneCapture2D
{
	GENERATED_BODY()


	AMiniCam();
	class ARBaseCharacter* Character;
	virtual void BeginPlay() override;
public:
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable,Category="My")
	void OutSideTick();
};

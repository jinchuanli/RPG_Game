// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,Category="SaveGame")
	int32 UserIndex;


	void SetUserIndex(int32 NewUserIndex);
	
	int32 GetUserIndex();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterInfo.generated.h"

USTRUCT()
struct FCharacterInfo:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="CharacterInfo")
	FString CharacterName;
	UPROPERTY(EditAnywhere,Category="CharacterInfo")
	float StartHP;
	UPROPERTY(EditAnywhere,Category="CharacterInfo")
	float StartMP;
};

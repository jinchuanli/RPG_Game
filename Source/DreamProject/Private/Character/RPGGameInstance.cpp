// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGGameInstance.h"

void URPGGameInstance::SetUserIndex(int32 NewUserIndex)
{
	UserIndex = NewUserIndex;
}

int32 URPGGameInstance::GetUserIndex()
{
	return UserIndex;
}

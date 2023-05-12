// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Revive.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"

bool URevive::Initialize()
{
	if(!Super::Initialize())
		return false;
	Revive->OnClicked.AddDynamic(this,&URevive::ClickOnRevive);
	return true;
}

void URevive::ClickOnRevive()
{
	CharacterRef->Respawn();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame/UserWidget/StartGameWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UStartGameWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	StartGame->OnClicked.AddDynamic(this,&UStartGameWidget::OpenLevel);
	ExitGame->OnClicked.AddDynamic(this,&UStartGameWidget::OnClickExitGame);
	return true;
}

void UStartGameWidget::OpenLevel()
{
	FString LevelName = "Ruined_Village_Demo";
	UGameplayStatics::OpenLevel(GetWorld(),FName(*LevelName));
}

void UStartGameWidget::OnClickExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit,false);
}


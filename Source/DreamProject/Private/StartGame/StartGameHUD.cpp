// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame/StartGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "StartGame/UserWidget/StartGameWidget.h"

AStartGameHUD::AStartGameHUD()
{
	static ConstructorHelpers::FClassFinder<UStartGameWidget> StartGameWidget_BPClass(TEXT("/Game/BluePrints/StartPage/UserWidget/MainStartWidget"));
	StartGameWidget = StartGameWidget_BPClass.Class;
}

void AStartGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if(StartGameWidget)
	{
		StartMain = CreateWidget<UStartGameWidget>(GetWorld(),StartGameWidget);
		StartMain->AddToViewport();
	}
}



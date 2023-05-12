// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame/StartGameMode.h"

#include "StartGame/StartGameHUD.h"
#include "StartGame/StartGamePawn.h"
#include "StartGame/StartPlayerController.h"
#include "StartGame/StartPlayerState.h"

AStartGameMode::AStartGameMode()
{
	HUDClass = AStartGameHUD::StaticClass();
	DefaultPawnClass = AStartGamePawn::StaticClass();
	PlayerControllerClass = AStartPlayerController::StaticClass();
	PlayerStateClass = AStartPlayerState::StaticClass();
}

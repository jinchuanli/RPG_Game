// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseGameModeBase.h"

#include "Character/RBaseCharacter.h"

ABaseGameModeBase::ABaseGameModeBase()
{
	// static ConstructorHelpers::FClassFinder<ARBaseCharacter> PlayerPawnBPClass(TEXT("/Game/BluePrints/Character/BP_RGideon"));
	// if(PlayerPawnBPClass.Class != nullptr)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }

	PlayerControllerClass = ABasePlayerController::StaticClass();
}

void ABaseGameModeBase::Respawn(ABasePlayerController* Controller)
{
	// if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass,CharacterRef->RevivepointLoction,CharacterRef->RevivepointRotation))
	// {
	// 	Controller->Possess(Pawn);
	// }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGame/StartGamePawn.h"

// Sets default values
AStartGamePawn::AStartGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStartGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

